#include "polygonBody.hpp"
#include "body.hpp"
#include "box2d/math_functions.h"
#include "polygon.hpp"

PolygonBody::PolygonBody(entt::registry &registry,
                         const std::shared_ptr<World> world,
                         const PolygonBodyConfig &config,
                         const std::shared_ptr<PhysicsFactory> physicsFactory)
    : Body(registry, world) {
  polygon = physicsFactory->createPolygon(config.shapeCfg);
  registerChild(polygon);
}

PolygonBodyConfig PolygonBodyConfig::defaultConfig() {
  PolygonBodyConfig ret;
  ret.shapeCfg = PolygonConfig::defaultConfig();
  return ret;
}

const std::shared_ptr<Polygon> PolygonBody::getPolygon(){
  return polygon;
}