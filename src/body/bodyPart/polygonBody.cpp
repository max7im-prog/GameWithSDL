#include "polygonBody.hpp"
#include "bodyPart.hpp"
#include "box2d/math_functions.h"
#include "polygon.hpp"

PolygonBody::PolygonBody(entt::registry &registry,
                         const std::shared_ptr<World> world,
                         const PolygonBodyConfig &config,
                         const std::shared_ptr<PhysicsFactory> physicsFactory)
    : BodyPart(registry, world) {
  polygon = physicsFactory->createPolygon(config.polygonConfig);
  registerShape(polygon);
}

PolygonBodyConfig PolygonBodyConfig::defaultConfig() {
  PolygonBodyConfig ret;
  ret.polygonConfig = PolygonConfig::defaultConfig();
  return ret;
}