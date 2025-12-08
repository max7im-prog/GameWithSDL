#include "capsule.hpp"
#include "box2d/types.h"
#include <box2d/box2d.h>

Capsule::Capsule(entt::registry &registry, const World &world,
                 const CapsuleConfig &config)
    : Shape(registry) {
  bodyId = b2CreateBody(world.getWorldId(), &(config.bodyDef));

  b2Capsule capsule;
  capsule.center1 = config.center1;
  capsule.center2 = config.center2;
  capsule.radius = config.radius;
  shapeId = b2CreateCapsuleShape(bodyId, &(config.shapeDef), &capsule);
}

void CapsuleConfig::defaultConfig() {
  shapeDef = b2DefaultShapeDef();
  bodyDef = b2DefaultBodyDef();
  radius = 1;
  center1 = {-0.5, 0};
  center2 = {0.5, 0};
}

b2Vec2 Capsule::getCenter1() {
  b2Capsule capsule = b2Shape_GetCapsule(shapeId);
  return b2Body_GetWorldPoint(bodyId, capsule.center1);
}

b2Vec2 Capsule::getCenter2() {
  b2Capsule capsule = b2Shape_GetCapsule(shapeId);
  return b2Body_GetWorldPoint(bodyId, capsule.center2);
}

b2Vec2 Capsule::getLocalCenter1() {
  return b2Shape_GetCapsule(shapeId).center1;
}

b2Vec2 Capsule::getLocalCenter2() {
  return b2Shape_GetCapsule(shapeId).center2;
}

float Capsule::getRadius() {
  b2Capsule capsule = b2Shape_GetCapsule(shapeId);
  return capsule.radius;
}
