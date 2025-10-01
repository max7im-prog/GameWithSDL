#include "capsuleBody.hpp"
#include "body.hpp"
#include "box2d/math_functions.h"
#include "capsule.hpp"
#include <stdexcept>

b2Vec2 CapsuleBody::getCenter1() {
  auto c = capsule.lock();
  if (!c)
    throw std::runtime_error("Capsule expired");
  return c->getCenter1();
}

b2Vec2 CapsuleBody::getCenter2() {
  auto c = capsule.lock();
  if (!c)
    throw std::runtime_error("Capsule expired");
  return c->getCenter2();
}

float CapsuleBody::getRadius() {
  auto c = capsule.lock();
  if (!c)
    throw std::runtime_error("Capsule expired");
  return c->getRadius();
}

CapsuleBody::CapsuleBody(entt::registry &registry,
                         const std::shared_ptr<World> world,
                         const CapsuleBodyConfig &config,
                         const std::shared_ptr<ShapeFactory> shapeFactory,
                         const std::shared_ptr<JointFactory> jointFactory)
    : Body(registry, world) {
  capsule = shapeFactory->create<Capsule>(config.shapeCfg);
  registerChild(capsule);
}

CapsuleBodyConfig CapsuleBodyConfig::defaultConfig() {
  CapsuleBodyConfig ret;
  ret.shapeCfg = CapsuleConfig::defaultConfig();
  return ret;
}

const std::weak_ptr<Capsule> CapsuleBody::getCapsule() { return capsule; }