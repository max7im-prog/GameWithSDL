#include "segmentTerrain.hpp"
#include "box2d/types.h"
#include "terrain.hpp"

SegmentTerrain::SegmentTerrain(
    entt::registry &registry, const std::shared_ptr<World> world,
    const SegmentTerrainConfig &config,
    const std::shared_ptr<PhysicsFactory> physicsFactory,
    const std::shared_ptr<BodyFactory> bodyFactory)
    : Terrain(registry, world) {

  // A single segment
  {
    auto bodyCfg = config.templateBodyCfg;
    bodyCfg.shapeCfg.localPoint1 = config.point1;
    bodyCfg.shapeCfg.localPoint2 = config.point2;
    bodyCfg.shapeCfg.bodyDef.position = config.position;
    bodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
    bodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
    segmentBody = bodyFactory->createSegmentBody(bodyCfg);
    registerChild(segmentBody);
  }
}

SegmentTerrainConfig SegmentTerrainConfig::defaultConfig() {
  SegmentTerrainConfig ret;
  ret.templateBodyCfg = SegmentBodyConfig::defaultConfig();
  ret.point1 = {0,0};
  ret.point1 = {1,0};
  ret.templateBodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
  ret.templateBodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
  ret.position = {0, 0};
  return ret;
}