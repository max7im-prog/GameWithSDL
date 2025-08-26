#include "polygonTerrain.hpp"
#include "box2d/types.h"
#include "polygonBody.hpp"
#include "terrain.hpp"

PolygonTerrain::PolygonTerrain(
    entt::registry &registry, const std::shared_ptr<World> world,
    const PolygonTerrainConfig &config,
    const std::shared_ptr<BodyFactory> bodyFactory,
    const std::shared_ptr<ConnectionFactory> connectionFactory)
    : Terrain(registry, world) {

  // A single polygon
  {
    auto bodyCfg = config.templateBodyCfg;
    bodyCfg.shapeCfg.vertices = config.vertices;
    bodyCfg.shapeCfg.bodyDef.position = config.position;
    bodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
    bodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
    polygonBody = bodyFactory->createPolygonBody(bodyCfg);
    registerChild(polygonBody);
  }
}

PolygonTerrainConfig PolygonTerrainConfig::defaultConfig() {
  PolygonTerrainConfig ret;
  ret.vertices = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
  ret.templateBodyCfg = PolygonBodyConfig::defaultConfig();
  ret.templateBodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
  ret.templateBodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
  ret.position = {0, 0};
  return ret;
}