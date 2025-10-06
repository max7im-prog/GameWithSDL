#include "polygon.hpp"
#include "box2d/types.h"
#include <box2d/box2d.h>

Polygon::Polygon(entt::registry &registry, const World &world,
                 const PolygonConfig &config)
    : Shape(registry) {
  bodyId = b2CreateBody(world.getWorldId(), &(config.bodyDef));

  b2Hull hull = b2ComputeHull(
      config.vertices.data(),
      std::min(int(config.vertices.size()), B2_MAX_POLYGON_VERTICES));
  b2Polygon polygon = b2MakePolygon(&hull, 0);
  shapeId = b2CreatePolygonShape(bodyId, &(config.shapeDef), &polygon);
}

PolygonConfig PolygonConfig::defaultConfig() {
  PolygonConfig ret;
  ret.shapeDef = b2DefaultShapeDef();
  ret.bodyDef = b2DefaultBodyDef();
  ret.radius = 0;
  ret.vertices = {{0, 0}, {0, 1}, {1, 1}, {1, 0}};
  return ret;
}

std::vector<b2Vec2> Polygon::getVertices() const {

  std::vector<b2Vec2> ret = {};
  auto p = b2Shape_GetPolygon(shapeId);
  ret.reserve(p.count);
  for (size_t i = 0; i < p.count; i++) {
    ret.push_back(p.vertices[i]);
  }
  return ret;
}