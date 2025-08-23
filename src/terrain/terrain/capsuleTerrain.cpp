#include "capsuleTerrain.hpp"
#include "box2d/types.h"
#include "terrain.hpp"

CapsuleTerrain::CapsuleTerrain(
    entt::registry &registry, const std::shared_ptr<World> world,
    const CapsuleTerrainConfig &config,
    const std::shared_ptr<PhysicsFactory> physicsFactory,
    const std::shared_ptr<BodyFactory> bodyFactory)
    : Terrain(registry, world) {

  // A single capsule
  {
    auto bodyCfg = config.templateBodyCfg;
    bodyCfg.shapeCfg.center1 = config.point1;
    bodyCfg.shapeCfg.center2 = config.point2;
    bodyCfg.shapeCfg.radius = config.radius;
    bodyCfg.shapeCfg.bodyDef.position = config.position;
    bodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
    bodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
    capsuleBody = bodyFactory->createCapsuleBody(bodyCfg);
    registerChild(capsuleBody);
  }
}

CapsuleTerrainConfig CapsuleTerrainConfig::defaultConfig() {
  CapsuleTerrainConfig ret;
  ret.templateBodyCfg = CapsuleBodyConfig::defaultConfig();
  ret.point1 = {0,0};
  ret.point1 = {1,0};
  ret.position = {0, 0};
  ret.radius = 0.25;
  ret.templateBodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
  ret.templateBodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
  return ret;
}