#include "segmentTerrain.hpp"
#include "box2d/types.h"
#include "polygonBody.hpp"
#include "terrain.hpp"

SegmentTerrain::SegmentTerrain(
    entt::registry &registry, const std::shared_ptr<World> world,
    const SegmentTerrainConfig &config,
    const std::shared_ptr<PhysicsFactory> physicsFactory,
    const std::shared_ptr<BodyFactory> bodyFactory)
    : Terrain(registry, world) {

  // A single segment
  {
    // auto cfg = SegmentBodyConfig::defaultConfig();
    // cfg.segmentConfig.
    // cfg.polygonConfig.bodyDef.position = config.position;
    // cfg.polygonConfig.vertices = config.polygonConfig.polygonConfig.vertices;
    // cfg.polygonConfig.bodyDef.type = b2_staticBody;
    // cfg.polygonConfig.shapeDef.filter = TerrainConfig::defaultFilter();
    // polygonBody = bodyFactory->createPolygonBody(cfg);
    // registerChild(polygonBody);
  }
}


SegmentTerrainConfig SegmentTerrainConfig::defaultConfig(){
  SegmentTerrainConfig ret;
  ret.segmentConfig = SegmentBodyConfig::defaultConfig();
  ret.segmentConfig.segmentConfig.bodyDef.type = b2_staticBody;
  ret.segmentConfig.segmentConfig.shapeDef.filter = TerrainConfig::defaultFilter();
  ret.position = {0,0};
  return ret;
}