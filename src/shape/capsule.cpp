#include "capsule.hpp"
#include "box2d/types.h"
#include <box2d/box2d.h>

Capsule::Capsule(const World &world, const CapsuleConfig &config) {
  bodyId = b2CreateBody(world.getWorldId(), &(config.bodyDef));

  b2Capsule capsule;
  capsule.center1 = config.center1;
  capsule.center2 = config.center2;
  capsule.radius = config.radius;
  shapeId = b2CreateCapsuleShape(bodyId, &(config.shapeDef), &capsule);
}

CapsuleConfig CapsuleConfig::defaultConfig() {
  CapsuleConfig ret;
  ret.shapeDef = b2DefaultShapeDef();
  ret.bodyDef = b2DefaultBodyDef();
  ret.radius = 1;
  ret.center1 = {-1, -1};
  ret.center2 = {1, 1};
  return ret;
}
