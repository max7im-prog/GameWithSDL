#include "circle.hpp"
#include "box2d/types.h"
#include <box2d/box2d.h>

Circle::Circle(entt::registry &registry, const World &world,
               const CircleConfig &config)
    : Shape(registry) {
  bodyId = b2CreateBody(world.getWorldId(), &(config.bodyDef));

  b2Circle circle;
  circle.center = {0, 0};
  circle.radius = config.radius;
  shapeId = b2CreateCircleShape(bodyId, &(config.shapeDef), &circle);
}

void CircleConfig::defaultConfig() {
  shapeDef = b2DefaultShapeDef();
  bodyDef = b2DefaultBodyDef();
  radius = 1;
}

void CircleConfig::fromJSON(const nlohmann::json& json){
  // TODO: implement
}

b2Vec2 Circle::getCenter() {
  b2Circle circle = b2Shape_GetCircle(shapeId);
  return b2Body_GetWorldPoint(bodyId, circle.center);
}

b2Vec2 Circle::getLocalCenter() {
  return b2Shape_GetCircle(shapeId).center;
}

float Circle::getRadius() {
  b2Circle circle = b2Shape_GetCircle(shapeId);
  return circle.radius;
}
