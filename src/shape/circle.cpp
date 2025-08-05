#include "circle.hpp"
#include <box2d/box2d.h>

Circle::Circle(const World &world, const CircleConfig &config) {
  bodyId = b2CreateBody(world.getWorldId(), &(config.bodyDef));

  b2Circle circle;
  circle.center = {0, 0};
  circle.radius = config.radius;
  shapeId = b2CreateCircleShape(bodyId, &(config.shapeDef), &circle);
}

