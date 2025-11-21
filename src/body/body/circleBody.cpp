#include "circleBody.hpp"
#include "body.hpp"
#include "box2d/math_functions.h"
#include "circle.hpp"

b2Vec2 CircleBody::getCenter() {

  auto c = circle.lock();
  if (!c)
    throw std::runtime_error("Circle expired");

  return c->getCenter();
}

float CircleBody::getRadius() {

  auto c = circle.lock();
  if (!c)
    throw std::runtime_error("Circle expired");

  return c->getRadius();
}

CircleBody::CircleBody(entt::registry &registry,
                       const std::shared_ptr<World> world,
                       const CircleBodyConfig &config,
                       const std::shared_ptr<ShapeFactory> shapeFactory,
                       const std::shared_ptr<JointFactory> jointFactory)
    : Body(registry, world) {
  circle = shapeFactory->create<Circle>(config.shapeCfg);
  registerShape(circle,"main");
}

void CircleBodyConfig::defaultConfig() { shapeCfg.defaultConfig(); }

const std::shared_ptr<Circle> CircleBody::getCircle() {

  auto ret = circle.lock();
  if (!ret)
    throw std::runtime_error("Shape expired");

  return ret;
}

b2Vec2 CircleBody::getWorldPos() { return circle.lock()->getWorldPos(); }