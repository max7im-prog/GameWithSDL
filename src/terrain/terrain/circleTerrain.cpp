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
    circleBody = bodyFactory->create<CircleBody>(bodyCfg);
    registerChild(circleBody);
  }
}

void CircleTerrainConfig::defaultConfig() {
  templateBodyCfg .defaultConfig();
  position = {0, 0};
  radius = 1;
  templateBodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
  templateBodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
}

void CircleTerrainConfig::fromJSON(const nlohmann::json& json){
  // TODO: implement
}