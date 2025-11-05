#include "segmentTerrain.hpp"
#include "box2d/math_functions.h"
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
  auto terrainPos = b2Add(config._transform._originPos, config._transform._relativePos);
  auto terrainRot = b2MulRot(config._transform._rootRot, config._transform._relativeRot);
  {
    auto bodyCfg = config.templateBodyCfg;
    bodyCfg.shapeCfg.localPoint1 = config.point1;
    bodyCfg.shapeCfg.localPoint2 = config.point2;
    bodyCfg.shapeCfg.bodyDef.position = terrainPos;
    bodyCfg.shapeCfg.bodyDef.rotation = terrainRot;
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
}

void SegmentTerrainConfig::fromJSON(const nlohmann::json& json){
  // TODO: implement
}