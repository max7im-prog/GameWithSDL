#include "circleBody.hpp"
#include "body.hpp"
#include "box2d/math_functions.h"
#include "circle.hpp"

b2Vec2 CircleBody::getCenter() { return circle->getCenter(); }

float CircleBody::getRadius() { return circle->getRadius(); }

CircleBody::CircleBody(entt::registry &registry,
                       const std::shared_ptr<World> world,
                       const CircleBodyConfig &config,
                       const std::shared_ptr<PhysicsFactory> physicsFactory)
    : Body(registry, world) {
  circle = physicsFactory->createCircle(config.shapeCfg);
  registerChild(circle);
}

CircleBodyConfig CircleBodyConfig::defaultConfig(){
  CircleBodyConfig ret;
  ret.shapeCfg = CircleConfig::defaultConfig();
  return ret;
}

const std::shared_ptr<Circle> CircleBody::getCircle(){
  return circle;
}