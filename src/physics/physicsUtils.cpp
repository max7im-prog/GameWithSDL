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

std::optional<b2Vec2> PhysicsUtils::getClosestPoint(const World &world,
                                                    b2Vec2 origin,
                                                    b2Vec2 translation,
                                                    b2QueryFilter filter) {
  auto res =
      b2World_CastRayClosest(world.getWorldId(), origin, translation, filter);
  std::optional<b2Vec2> ret = std::nullopt;
  if (res.hit) {
   ret = res.point;
  } 
  return ret;
}