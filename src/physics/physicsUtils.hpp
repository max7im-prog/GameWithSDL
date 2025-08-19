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
                                               b2Vec2 origin,
                                               b2Vec2 translation,
                                               b2QueryFilter filter);

private:
  /**
   * @brief Used in getShapeAtPosition to get a callback from a
   * b2World_OverlapAABB.
   *
   * @param context A pointer to std::vector<b2ShapeId>
   */
  static bool pointOverlapCallbackFunction(b2ShapeId shapeId, void *context);
};

enum ObjectCategory {
  GENERAL = 1 << 0,
  CREATURE = 1 << 1,
  TERRAIN = 1 << 2,
};