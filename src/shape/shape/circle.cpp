#include "circle.hpp"
#include "box2d/types.h"
#include <box2d/box2d.h>

Circle::Circle(const World &world, const CircleConfig &config) {
  bodyId = b2CreateBody(world.getWorldId(), &(config.bodyDef));

  b2Circle circle;
  circle.center = {0, 0};
  circle.radius = config.radius;
  shapeId = b2CreateCircleShape(bodyId, &(config.shapeDef), &circle);
}

CircleConfig CircleConfig::defaultConfig(){
    CircleConfig ret;
    ret.shapeDef = b2DefaultShapeDef();
    ret.bodyDef = b2DefaultBodyDef();
    ret.radius = 1;
    return ret;
}

