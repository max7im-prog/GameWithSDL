#include "limbBody.hpp"
#include "PIDVectorController.hpp"
#include "box2d/box2d.h"
#include "box2d/math_functions.h"
#include "capsule.hpp"
#include "kinematicUtils.hpp"
#include "revoluteConnection.hpp"
#include "revoluteJoint.hpp"

LimbBody::LimbBody(entt::registry &registry, const std::shared_ptr<World> world,
                   const LimbBodyConfig &c,
                   const std::shared_ptr<ShapeFactory> shapeFactory,
                   const std::shared_ptr<JointFactory> jointFactory)
    : Body(registry, world), config(c) {

  b2Vec2 lastPos = config.basePos;
  const b2Vec2 incrementDir = {1, 0};

  // Create capsules
  segmentLengths = {};
  length = 0;
  for (auto seg : config.segments) {
    auto capsuleConfig = config.templateCapsuleConfig;
    capsuleConfig.bodyDef.position = lastPos;
    b2Vec2 offset = b2Sub(seg.endPos, lastPos);
    float len = b2Length(offset);
    b2Vec2 norm = b2Normalize(offset);
    float angle = b2Atan2(norm.y, norm.x);

    b2Rot rot = b2MakeRot(angle);
    segmentLengths.push_back(len);
    length += len;
    capsuleConfig.bodyDef.rotation = rot;
    capsuleConfig.center1 = {0, 0};
    capsuleConfig.center2 = b2MulSV(len, incrementDir);
    capsuleConfig.radius = seg.radius;
    auto capsule = shapeFactory->create<Capsule>(capsuleConfig);
    registerChild(capsule);
    segments.push_back(capsule);
    lastPos = capsule->getCenter2();
  }

  // Configure Inverse Kinematics template
  rootRot = config.rootRot;
  if (config.initialAngleConstraints.size() != 0) {
    rootIKTask.angleConstraints = config.initialAngleConstraints;
  } else {
    rootIKTask.angleConstraints =
        std::vector<AngleConstraint>(segmentLengths.size());
  }
  rootIKTask.lengths = segmentLengths;

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

    jointConfig.jointDef.enableLimit = config.initialAngleConstraints[i].enable;
    jointConfig.jointDef.lowerAngle = config.initialAngleConstraints[i].minRot;
    jointConfig.jointDef.upperAngle = config.initialAngleConstraints[i].maxRot;
    jointConfig.jointDef.referenceAngle = 0.0f;

    auto joint = jointFactory->create<RevoluteJoint>(jointConfig);
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
}

LimbBodyConfig LimbBodyConfig::defaultConfig() {
  LimbBodyConfig ret;
  ret.templateCapsuleConfig = CapsuleConfig::defaultConfig();
  ret.templateJointConfig = RevoluteJointConfig::defaultConfig();
  ret.basePos = {0, 0};
  ret.segments = {};

  ret.initialAngleConstraints = {};

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
  rootIKTask.previousJoints = oldPos;
  rootIKTask.fixturePoint = getBasePos();
  rootIKTask.targetPoint = trackingContext.trackingPoint;
  rootIKTask.rootRot = getAdjustedRootRot();

  auto newPos = rootIKTask.solveFABRIK();

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

void LimbBody::setAngleConstraints(
    const std::vector<AngleConstraint> &constraints) {
  if (constraints.size() != segments.size()) {
    throw(
        std::runtime_error("Angle constraint of different size than segments"));
  }

  // Update inverse kinematics
  rootIKTask.angleConstraints = constraints;

  // Update joints
  if (connection) {
    auto joint = connection->getRevoluteJoint();
    joint->setAngleLimits(rootIKTask.angleConstraints[0].minRot,
                          rootIKTask.angleConstraints[0].maxRot);
  }

  for (size_t i = 0; i < joints.size(); i++) {
    auto joint = joints[i];

    joint->setAngleLimits(rootIKTask.angleConstraints[i + 1].minRot,
                          rootIKTask.angleConstraints[i + 1].maxRot);
  }
}

void LimbBody::connect(std::shared_ptr<ConnectionFactory> factory,
                       std::shared_ptr<Shape> shape, b2Vec2 localPoint) {
  if (!shape) {
    throw(std::runtime_error("Nullptr instead of shape"));
  }
  if (connection) {
    connection->remove();
    unregisterChild(connection);
    connection = nullptr;
  }

  {
    auto cfg = RevoluteConnectionConfig::defaultConfig();
    cfg.templateJointCfg = config.templateJointConfig;
    cfg.templateJointCfg.jointDef.bodyIdA = shape->getBodyId();
    cfg.templateJointCfg.jointDef.bodyIdB = segments[0]->getBodyId();
    cfg.templateJointCfg.jointDef.localAnchorA = localPoint;
    cfg.templateJointCfg.jointDef.localAnchorB = {0, 0};
    cfg.templateJointCfg.jointDef.enableLimit =
        rootIKTask.angleConstraints[0].enable;
    cfg.templateJointCfg.jointDef.lowerAngle =
        rootIKTask.angleConstraints[0].minRot;
    cfg.templateJointCfg.jointDef.upperAngle =
        rootIKTask.angleConstraints[0].maxRot;
    cfg.templateJointCfg.jointDef.referenceAngle =
        b2Rot_GetAngle(b2MulRot(rootRot, shape->getRotation()));

    connection = factory->create<RevoluteConnection>(cfg);
    registerChild(connection);
  }
}

b2Rot LimbBody::getAdjustedRootRot() {
  b2Rot ret;

  if (connection) {
    ret = b2MulRot(rootRot, b2Body_GetRotation(b2Joint_GetBodyA(
                                connection->getRevoluteJoint()->getJointId())));
  } else {
    b2Vec2 v = b2Normalize(
        b2Sub(segments[0]->getCenter1(), segments[0]->getCenter2()));
    b2Rot r = {.c = v.y, .s = v.x};
    ret = r;
  }

  return ret;
}