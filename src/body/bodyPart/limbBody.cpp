#include "limbBody.hpp"
#include "PIDVectorController.hpp"
#include "box2d/box2d.h"
#include "box2d/math_functions.h"
#include "capsule.hpp"
#include "kinematicUtils.hpp"
#include "revoluteJoint.hpp"

LimbBody::LimbBody(entt::registry &registry, const std::shared_ptr<World> world,
                   const LimbBodyConfig &config,
                   const std::shared_ptr<PhysicsFactory> physicsFactory)
    : Body(registry, world), segmentsConfig(config.segments) {

  b2Vec2 lastPos = config.basePos;
  b2Vec2 incrementDir = {0, -1};

  // Create capsules
  auto capsuleConfig = config.templateCapsuleConfig;
  for (auto seg : config.segments) {
    capsuleConfig.bodyDef.position = lastPos;
    capsuleConfig.bodyDef.rotation = config.rotation;
    capsuleConfig.center1 = {0, 0};
    capsuleConfig.center2 = b2MulSV(seg.len, incrementDir);
    capsuleConfig.radius = seg.radius;
    auto capsule = physicsFactory->createCapsule(capsuleConfig);
    registerChild(capsule);
    segments.push_back(capsule);
    lastPos = capsule->getCenter2();
  }

  // Connect capsules
  auto jointConfig = config.templateJointConfig;
  for (size_t i = 1; i < segments.size(); i++) {
    auto bodyA = segments[i - 1]->getBodyId();
    auto bodyB = segments[i]->getBodyId();

    jointConfig.jointDef.bodyIdA = bodyA;
    jointConfig.jointDef.bodyIdB = bodyB;
    jointConfig.jointDef.localAnchorA =
        b2Body_GetLocalPoint(bodyA, segments[i - 1]->getCenter2());
    jointConfig.jointDef.localAnchorB =
        b2Body_GetLocalPoint(bodyB, segments[i]->getCenter1());

    auto joint = physicsFactory->createRevoluteJoint(jointConfig);
    registerChild(joint);
    joints.push_back(joint);
  }

  // Create PID controllers
  constexpr int CONTROLLERS_PER_SEGMENT = 2;
  float gravity = b2Length(b2World_GetGravity(world->getWorldId()));
  for (auto capsule : segments) {
    float mass = capsule->getMass();
    float force = mass * gravity / CONTROLLERS_PER_SEGMENT;
    {
      PIDVectorControllerConfig cfg;
      cfg.constant = b2Vec2(0, 0);
      cfg.kd = force * config.limbControlConfig.KDMultiplier;
      cfg.kp = force * config.limbControlConfig.KPMultiplier;
      cfg.ki = force * config.limbControlConfig.KIMultiplier;
      cfg.maxForce = force * config.limbControlConfig.maxForceMultiplier;
      PIDVectorController first(cfg);
      PIDVectorController second(cfg);
      controllers.push_back({.baseController = first, .endController = second});
    }
  }

  // Compute lengths and other variables
  segmentLengths = {};
  length = 0;
  for (auto el : segmentsConfig) {
    segmentLengths.push_back(el.len);
    length += el.len;
  }
}

LimbBodyConfig LimbBodyConfig::defaultConfig() {
  LimbBodyConfig ret;
  ret.templateCapsuleConfig = CapsuleConfig::defaultConfig();
  ret.templateJointConfig = RevoluteJointConfig::defaultConfig();
  ret.basePos = {0, 0};
  ret.rotation = b2MakeRot(0);
  ret.segments = {};

  return ret;
}

b2Vec2 LimbBody::getBasePos() {
  if (segments.size() == 0) {
    return {0, 0};
  }
  return segments.begin()->get()->getCenter1();
}

b2Vec2 LimbBody::getEndPos() {
  if (segments.size() == 0) {
    return {0, 0};
  }
  return segments.back()->getCenter2();
}

const std::vector<float> &LimbBody::getSegmentLengths() {
  return segmentLengths;
}

float LimbBody::getLength() { return length; };

void LimbBody::setTracking(b2Vec2 worldPoint, bool isTracking) {
  this->trackingContext.trackingPoint = worldPoint;
  this->trackingContext.isTracking = isTracking;
}

bool LimbBody::getTracking() { return trackingContext.isTracking; }

b2Vec2 LimbBody::getTrackingPoint() { return trackingContext.trackingPoint; }

std::vector<b2Vec2> LimbBody::getJointsPos() {
  if (segments.empty()) {
    return {};
  }
  std::vector<b2Vec2> ret = {};
  for (auto capsule : segments) {
    ret.push_back(capsule->getCenter1());
  }
  ret.push_back(segments.back()->getCenter2());
  return ret;
}

void LimbBody::update(float dt) {
  if (!trackingContext.isTracking) {
    return;
  }

  auto oldPos = this->getJointsPos();

  auto newPos = KinematicUtils::solveFABRIK(
      getBasePos(), trackingContext.trackingPoint, oldPos, getSegmentLengths());

  b2Vec2 correctingForce = {0, 0};
  b2Vec2 firstError = {0, 0};
  b2Vec2 secondError = b2Sub(newPos[0], oldPos[0]);
  for (size_t i = 0; i < controllers.size(); i++) {
    firstError = secondError;
    secondError = b2Sub(newPos[i + 1], oldPos[i + 1]);
    b2Vec2 firstForce = controllers[i].baseController.update(firstError, dt);
    b2Vec2 secondForce = controllers[i].endController.update(secondError, dt);
    b2BodyId bodyId = segments[i]->getBodyId();
    b2Body_ApplyForce(bodyId, firstForce, oldPos[i], true);
    b2Body_ApplyForce(bodyId, secondForce, oldPos[i + 1], true);
    correctingForce = b2Sub(correctingForce, b2Add(firstForce, secondForce));
  }

  // Apply correcting force to stop limb from moving body it is connected to
  b2Body_ApplyForce(segments[0]->getBodyId(), correctingForce, getBasePos(),
                    true);
}

const std::vector<std::shared_ptr<Capsule>> &LimbBody::getSegments() const {
  return segments;
}