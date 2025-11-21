#include "capsuleTerrain.hpp"
#include "box2d/types.h"
#include "jsonUtils.hpp"
#include "terrain.hpp"

CapsuleTerrain::CapsuleTerrain(
    entt::registry &registry, const std::shared_ptr<World> world,
    const CapsuleTerrainConfig &config,
    const std::shared_ptr<BodyFactory> bodyFactory,
    const std::shared_ptr<ConnectionFactory> connectionFactory)
    : Terrain(registry, world) {

  // A single capsule
  auto terrainPos =
      b2Add(config._transform._originPos, config._transform._relativePos);
  auto terrainRot =
      b2MulRot(config._transform._rootRot, config._transform._relativeRot);

  std::vector<b2Vec2> transformedVertices = {config.point1,config.point2};
  std::for_each(transformedVertices.begin(), transformedVertices.end(),
                [&config](b2Vec2 &v) {
                  v.x = config._transform._scaleX * v.x *
                        (config._transform._flipX ? -1 : 1);
                  v.y = config._transform._scaleY * v.y *
                        (config._transform._flipY ? -1 : 1);
                });
  {
    auto bodyCfg = config.templateBodyCfg;
    bodyCfg.shapeCfg.center1 = transformedVertices[0];
    bodyCfg.shapeCfg.center2 = transformedVertices[1];
    bodyCfg.shapeCfg.radius = config.radius * std::min(config._transform._scaleX, config._transform._scaleY);
    bodyCfg.shapeCfg.bodyDef.position = terrainPos;
    bodyCfg.shapeCfg.bodyDef.rotation = terrainRot;
    bodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
    capsuleBody = bodyFactory->create<CapsuleBody>(bodyCfg);
    registerBody(capsuleBody,"capsuleBody");
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
  defaultConfig();
  if (json.contains("point1")) {
    point1 = JsonUtils::parseB2Vec2(json["point1"]);
  } else {
    // TODO: log error
  }

  if (json.contains("point2")) {
    point2 = JsonUtils::parseB2Vec2(json["point2"]);
  } else {
    // TODO: log error
  }

  if (b2Distance(point1, point2) == 0) {
    // TODO: log error (segments of length zero lead to crashes)
    point1 = {0, 0};
    point2 = {1, 0};
  }

  radius = JsonUtils::getOrDefault<float>(json, "radius", 1.0f);

  if(json.contains("bodyParams")){
    auto bodyParams = TerrainConfig::parseBodyParams(json["bodyParams"]);
    templateBodyCfg.shapeCfg.bodyDef = bodyParams._bodyDef;
    templateBodyCfg.shapeCfg.shapeDef = bodyParams._shapeDef;
  }


  // TODO: remove testing code
  _renderConfig = std::make_shared<TopLevelRenderConfig>();
}

b2Vec2 CapsuleTerrain::getWorldPos() { return capsuleBody.lock()->getWorldPos(); }