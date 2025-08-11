#pragma once

#include "box2d/types.h"
#include <entt/entt.hpp>

#include "world.hpp"
#include <box2d/box2d.h>

class PhysicsUtils {
public:
  static std::vector<b2ShapeId> getShapeAtPosition(b2WorldId worldId,
                                                   b2Vec2 position);

  static std::optional<b2Vec2> getClosestPoint(const World &world,
                                               b2Vec2 beginPoint,
                                               b2Vec2 endPoint,
                                               b2QueryFilter filter);

private:
  // Used in getShapeAtPosition to get a callback from a b2World_OverlapAABB.
  // Context should be a pointer to std::vector<b2ShapeId>
  static bool pointOverlapCallbackFunction(b2ShapeId shapeId, void *context);
};