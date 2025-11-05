#include "capsuleTerrain.hpp"
#include "box2d/types.h"
#include "terrain.hpp"

CapsuleTerrain::CapsuleTerrain(
    entt::registry &registry, const std::shared_ptr<World> world,
    const CapsuleTerrainConfig &config,
    const std::shared_ptr<BodyFactory> bodyFactory,
    const std::shared_ptr<ConnectionFactory> connectionFactory)
    : Terrain(registry, world) {

  // A single capsule
  auto terrainPos = b2Add(config._transform._originPos, config._transform._relativePos);
  auto terrainRot = b2MulRot(config._transform._rootRot, config._transform._relativeRot);
  {
    auto bodyCfg = config.templateBodyCfg;
    bodyCfg.shapeCfg.center1 = config.point1;
    bodyCfg.shapeCfg.center2 = config.point2;
    bodyCfg.shapeCfg.radius = config.radius;
    bodyCfg.shapeCfg.bodyDef.position = terrainPos;
    bodyCfg.shapeCfg.bodyDef.rotation = terrainRot;
    bodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
    bodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
    capsuleBody = bodyFactory->create<CapsuleBody>(bodyCfg);
    registerChild(capsuleBody);
  }
}

void CapsuleTerrainConfig::defaultConfig() {
  templateBodyCfg.defaultConfig();
  point1 = {0, 0};
  point2 = {1, 0};
  radius = 0.25;
  templateBodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
  templateBodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
}

void CapsuleTerrainConfig::fromJSON(const nlohmann::json &json) {
  // TODO: implement
}