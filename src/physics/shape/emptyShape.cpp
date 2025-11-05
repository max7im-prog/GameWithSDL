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

void EmptyShapeConfig::defaultConfig() {
  shapeDef = b2DefaultShapeDef();
  bodyDef = b2DefaultBodyDef();
  bodyDef.type = b2_staticBody;
}
