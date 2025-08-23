#include "polygonTerrain.hpp"
#include "box2d/types.h"
#include "polygonBody.hpp"
#include "terrain.hpp"

PolygonTerrain::PolygonTerrain(
    entt::registry &registry, const std::shared_ptr<World> world,
    const PolygonTerrainConfig &config,
    const std::shared_ptr<PhysicsFactory> physicsFactory,
    const std::shared_ptr<BodyFactory> bodyFactory)
    : Terrain(registry, world) {

  // A single polygon
  {
    auto bodyCfg = PolygonBodyConfig::defaultConfig();

    bodyCfg.polygonConfig.bodyDef.position = config.position;
    bodyCfg.polygonConfig.vertices = config.polygonConfig.polygonConfig.vertices;
    bodyCfg.polygonConfig.bodyDef.type = b2_staticBody;
    bodyCfg.polygonConfig.shapeDef.filter = TerrainConfig::defaultFilter();
    polygonBody = bodyFactory->createPolygonBody(bodyCfg);
    registerChild(polygonBody);
  }
}


PolygonTerrainConfig PolygonTerrainConfig::defaultConfig(){
  PolygonTerrainConfig ret;
  ret.polygonConfig = PolygonBodyConfig::defaultConfig();
  ret.polygonConfig.polygonConfig.bodyDef.type = b2_staticBody;
  ret.polygonConfig.polygonConfig.shapeDef.filter = TerrainConfig::defaultFilter();
  ret.position = {0,0};
  return ret;
}