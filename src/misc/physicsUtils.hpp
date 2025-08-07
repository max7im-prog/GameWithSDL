#pragma once

#include <entt/entt.hpp>

#include <box2d/box2d.h>

class PhysicsUtils {
public:
  static std::vector<b2ShapeId> getShapeAtPosition(b2WorldId worldId,
                                                   b2Vec2 position);

private:
  // Used in getShapeAtPosition to get a callback from a b2World_OverlapAABB.
  // Context should be a pointer to std::vector<b2ShapeId>
  static bool pointOverlapCallbackFunction(b2ShapeId shapeId, void *context);
};