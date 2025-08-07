#include "physicsUtils.hpp"
#include "box2d/types.h"

#include <box2d/box2d.h>
#include <entt/entt.hpp>

std::vector<b2ShapeId> PhysicsUtils::getShapeAtPosition(b2WorldId worldId,
                                                        b2Vec2 position) {
  std::vector<b2ShapeId> potential;
  std::vector<b2ShapeId> result;

  const float epsilon = 0.001f;
  b2AABB aabb;
  aabb.lowerBound = {position.x - epsilon, position.y - epsilon};
  aabb.upperBound = {position.x + epsilon, position.y + epsilon};

  b2QueryFilter filter = b2DefaultQueryFilter();
  // filter.maskBits = 0xFFFF;
  // filter.maskBits = filter.categoryBits;

  b2World_OverlapAABB(worldId, aabb, filter,
                      &PhysicsUtils::pointOverlapCallbackFunction, &potential);

  for (auto shape : potential) {
    if (b2Shape_TestPoint(shape, position)) {
      result.push_back(shape);
    }
  }

  return result;
}

bool PhysicsUtils::pointOverlapCallbackFunction(b2ShapeId shapeId,
                                                void *context) {
  std::vector<b2ShapeId> *shapes =
      static_cast<std::vector<b2ShapeId> *>(context);
  shapes->push_back(shapeId);
  return true;
}
