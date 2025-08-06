#include "circleBody.hpp"
#include "bodyPart.hpp"
#include "box2d/math_functions.h"
#include "circle.hpp"

b2Vec2 CircleBody::getCenter() { return circle->getCenter(); }

float CircleBody::getRadius() { return circle->getRadius(); }

CircleBody::CircleBody(entt::registry &registry,
                       const std::shared_ptr<World> world,
                       const CircleBodyConfig &config,
                       const std::shared_ptr<PhysicsFactory> physicsFactory)
    : BodyPart(registry, world) {
  circle = physicsFactory->createCircle(config.circleConfig);
}

CircleBodyConfig CircleBodyConfig::defaultConfig(){
  CircleBodyConfig ret;
  ret.circleConfig = CircleConfig::defaultConfig();
  return ret;
}