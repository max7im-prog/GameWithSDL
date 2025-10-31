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
  registerChild(circle);
}

void CircleBodyConfig::defaultConfig() {
  shapeCfg.defaultConfig();
}

void SegmentConfig::fromJSON(const nlohmann::json& json){
  // TODO: implement
}

const std::shared_ptr<Circle> CircleBody::getCircle() { 
  
  auto ret = circle.lock();
  if (!ret)
    throw std::runtime_error("Shape expired");
  
  return ret; }