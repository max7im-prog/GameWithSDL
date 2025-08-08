#include "capsuleBody.hpp"
#include "bodyPart.hpp"
#include "box2d/math_functions.h"
#include "capsule.hpp"

b2Vec2 CapsuleBody::getCenter1() { return capsule->getCenter1(); }

b2Vec2 CapsuleBody::getCenter2() { return capsule->getCenter2(); }

float CapsuleBody::getRadius() { return capsule->getRadius(); }

CapsuleBody::CapsuleBody(entt::registry &registry,
                       const std::shared_ptr<World> world,
                       const CapsuleBodyConfig &config,
                       const std::shared_ptr<PhysicsFactory> physicsFactory)
    : BodyPart(registry, world) {
  capsule = physicsFactory->createCapsule(config.capsuleConfig);
  registerShape(capsule);
}

CapsuleBodyConfig CapsuleBodyConfig::defaultConfig(){
  CapsuleBodyConfig ret;
  ret.capsuleConfig = CapsuleConfig::defaultConfig();
  return ret;
}