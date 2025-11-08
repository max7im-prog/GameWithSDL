#include "polygonBody.hpp"
#include "body.hpp"
#include "polygon.hpp"

PolygonBody::PolygonBody(entt::registry &registry,
                         const std::shared_ptr<World> world,
                         const PolygonBodyConfig &config,
                         const std::shared_ptr<ShapeFactory> shapeFactory,
                         const std::shared_ptr<JointFactory> jointFactory)
    : Body(registry, world) {
  polygon = shapeFactory->create<Polygon>(config.shapeCfg);
  registerChild(polygon);
}

void PolygonBodyConfig::defaultConfig() {
  shapeCfg.defaultConfig();
}


const std::shared_ptr<Polygon> PolygonBody::getPolygon() {

  auto ret = polygon.lock();
  if (!ret)
    throw std::runtime_error("Shape expired");

  return ret;
}

b2Vec2 PolygonBody::getWorldPos(){
  return polygon.lock()->getWorldPos();
}