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

void CapsuleBodyConfig::defaultConfig() {
  shapeCfg .defaultConfig();
}

void SegmentConfig::fromJSON(const nlohmann::json& json){
  // TODO: implement
}

const std::shared_ptr<Capsule> CapsuleBody::getCapsule() {

  auto ret = capsule.lock();
  if (!ret)
    throw std::runtime_error("Shape expired");
  return ret;
}