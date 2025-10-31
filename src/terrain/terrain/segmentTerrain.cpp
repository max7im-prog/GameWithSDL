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

void SegmentTerrainConfig::defaultConfig() {
  templateBodyCfg .defaultConfig();
  point1 = {0, 0};
  point2 = {1, 0};
  templateBodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
  templateBodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
  position = {0, 0};
}

void SegmentTerrainConfig::fromJSON(const nlohmann::json& json){
  // TODO: implement
}