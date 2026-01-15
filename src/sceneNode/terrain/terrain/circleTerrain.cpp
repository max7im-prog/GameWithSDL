#include "circleTerrain.hpp"
#include "box2d/types.h"
#include "jsonUtils.hpp"
#include "terrain.hpp"

CircleTerrain::CircleTerrain(
    entt::registry &registry, const std::shared_ptr<World> world,
    const CircleTerrain::Config &config,
    const std::shared_ptr<BodyFactory> bodyFactory,
    const std::shared_ptr<ConnectionFactory> connectionFactory)
    : Terrain(registry, world) {

  // A single circle
  auto terrainPos =
      b2Add(config._transform._originPos, config._transform._relativePos);
  auto terrainRot = b2MulRot(b2MakeRot(config._transform._rootRotRad),
                             b2MakeRot(config._transform._relativeRotRad));
  {
    auto bodyCfg = config.templateBodyCfg;
    bodyCfg.shapeCfg.radius =
        config.radius *
        std::max(config._transform._scaleX, config._transform._scaleY);
    bodyCfg.shapeCfg.bodyDef.position = terrainPos;
    bodyCfg.shapeCfg.bodyDef.rotation = terrainRot;
    bodyCfg.shapeCfg.shapeDef.filter = Terrain::Config::defaultFilter();
    circleBody = bodyFactory->create<CircleBody>(bodyCfg);
    registerBody(circleBody, "main");
  }
}

void CircleTerrain::Config::defaultConfig() {
  templateBodyCfg.defaultConfig();
  radius = 1;
  templateBodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
  templateBodyCfg.shapeCfg.shapeDef.filter = Terrain::Config::defaultFilter();
}

void CircleTerrain::Config::fromJSON(const nlohmann::json &json) {
  defaultConfig();
  radius = JsonUtils::getOrDefault<float>(json, "radius", 1.0f);

  if (json.contains("bodyParams")) {
    auto bodyParams = Terrain::Config::parseBodyParams(json["bodyParams"]);
    templateBodyCfg.shapeCfg.bodyDef = bodyParams._bodyDef;
    templateBodyCfg.shapeCfg.shapeDef = bodyParams._shapeDef;
  }

  if (json.contains("renderConfig")) {
    _renderConfig = SceneNode::Config::parseRenderConfig(json["renderConfig"]);
  } else {
    _renderConfig = nullptr;
  }
}
b2Vec2 CircleTerrain::getWorldPos() { return circleBody.lock()->getWorldPos(); }
