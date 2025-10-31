#include "segmentTerrain.hpp"
#include "box2d/types.h"
#include "jsonUtils.hpp"
#include "terrain.hpp"

SegmentTerrain::SegmentTerrain(
    entt::registry &registry, const std::shared_ptr<World> world,
    const SegmentTerrainConfig &config,
    const std::shared_ptr<BodyFactory> bodyFactory,
    const std::shared_ptr<ConnectionFactory> connectionFactory)
    : Terrain(registry, world) {

  // A single segment
  {
    auto bodyCfg = config.templateBodyCfg;
    bodyCfg.shapeCfg.localPoint1 = config.point1;
    bodyCfg.shapeCfg.localPoint2 = config.point2;
    bodyCfg.shapeCfg.bodyDef.position = config.position;
    bodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
    bodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
    segmentBody = bodyFactory->create<SegmentBody>(bodyCfg);
    registerChild(segmentBody);
  }
}

SegmentTerrainConfig SegmentTerrainConfig::defaultConfig() {
  SegmentTerrainConfig ret;
  ret.templateBodyCfg = SegmentBodyConfig::defaultConfig();
  ret.point1 = {0, 0};
  ret.point2 = {1, 0};
  ret.templateBodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
  ret.templateBodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
  ret.position = {0, 0};
  return ret;
}

SegmentTerrainConfig
SegmentTerrainConfig::fromJSON(const std::string &filename) {
  SegmentTerrainConfig ret;
  auto temp = JsonUtils::parseJSON(filename);
  if (!temp) {
    throw std::runtime_error("Failed to parse json file: " + filename);
  }
  auto json = *temp;
  ret = SegmentTerrainConfig::defaultConfig();
  

  return ret;
}
