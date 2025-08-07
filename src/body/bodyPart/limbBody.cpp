#include "limbBody.hpp"
#include "box2d/box2d.h"
#include "box2d/math_functions.h"
#include "capsule.hpp"
#include "revoluteJoint.hpp"

LimbBody::LimbBody(entt::registry &registry, const std::shared_ptr<World> world,
                   const LimbBodyConfig &config,
                   const std::shared_ptr<PhysicsFactory> physicsFactory)
    : BodyPart(registry, world) {

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
    joints.push_back(joint);
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
  return segments.end()->get()->getCenter2();
}

void LimbBody::aimAt(b2Vec2 worldPoint) { /* TODO: implement */ }
void LimbBody::update(float dt) { /* TODO: implement */ }