#include "circleTerrain.hpp"
#include "box2d/types.h"
#include "terrain.hpp"

CircleTerrain::CircleTerrain(
    entt::registry &registry, const std::shared_ptr<World> world,
    const CircleTerrainConfig &config,
    const std::shared_ptr<BodyFactory> bodyFactory,
    const std::shared_ptr<ConnectionFactory> connectionFactory)
    : Terrain(registry, world) {

  // A single circle
  {
    auto bodyCfg = config.templateBodyCfg;
    bodyCfg.shapeCfg.radius = config.radius;
    bodyCfg.shapeCfg.bodyDef.position = config.position;
    bodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
    bodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
    circleBody = bodyFactory->createCircleBody(bodyCfg);
    registerChild(circleBody);
  }
}

CircleTerrainConfig CircleTerrainConfig::defaultConfig() {
  CircleTerrainConfig ret;
  ret.templateBodyCfg = CircleBodyConfig::defaultConfig();
  ret.position = {0, 0};
  ret.radius = 1;
  ret.templateBodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
  ret.templateBodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
  return ret;
}