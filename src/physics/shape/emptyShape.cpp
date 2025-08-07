#include "emptyShape.hpp"
#include "box2d/id.h"
#include "box2d/types.h"
#include <box2d/box2d.h>

EmptyShape::EmptyShape(entt::registry &registry, const World &world,
                       const EmptyShapeConfig &config)
    : Shape(registry) {
  bodyId = b2CreateBody(world.getWorldId(), &(config.bodyDef));
  shapeId = b2_nullShapeId;
}

EmptyShapeConfig EmptyShapeConfig::defaultConfig() {
  EmptyShapeConfig ret;
  ret.shapeDef = b2DefaultShapeDef();
  ret.bodyDef = b2DefaultBodyDef();
  ret.bodyDef.type = b2_staticBody;
  return ret;
}
