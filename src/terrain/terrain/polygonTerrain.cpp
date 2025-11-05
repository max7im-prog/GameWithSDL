#include "polygonTerrain.hpp"
#include "box2d/types.h"
#include "jsonUtils.hpp"
#include "polygonBody.hpp"
#include "terrain.hpp"

PolygonTerrain::PolygonTerrain(
    entt::registry &registry, const std::shared_ptr<World> world,
    const PolygonTerrainConfig &config,
    const std::shared_ptr<BodyFactory> bodyFactory,
    const std::shared_ptr<ConnectionFactory> connectionFactory)
    : Terrain(registry, world) {

  // A single polygon
  auto terrainPos =
      b2Add(config._transform._originPos, config._transform._relativePos);
  auto terrainRot =
      b2MulRot(config._transform._rootRot, config._transform._relativeRot);

  auto transformedVertices = config.vertices;
  std::for_each(transformedVertices.begin(), transformedVertices.end(),
                [&config](b2Vec2 &v) {
                  v.x = config._transform._scaleX * v.x *
                        (config._transform._flipX ? -1 : 1);
                  v.y = config._transform._scaleY * v.y *
                        (config._transform._flipY ? -1 : 1);
                });
  {
    auto bodyCfg = config.templateBodyCfg;
    bodyCfg.shapeCfg.vertices = transformedVertices;
    bodyCfg.shapeCfg.bodyDef.position = terrainPos;
    bodyCfg.shapeCfg.bodyDef.rotation = terrainRot;
    bodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
    bodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
    polygonBody = bodyFactory->create<PolygonBody>(bodyCfg);
    registerChild(polygonBody);
  }
}

void PolygonTerrainConfig::defaultConfig() {
  vertices = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
  templateBodyCfg.defaultConfig();
  templateBodyCfg.shapeCfg.bodyDef.type = b2_staticBody;
  templateBodyCfg.shapeCfg.shapeDef.filter = TerrainConfig::defaultFilter();
}

void PolygonTerrainConfig::fromJSON(const nlohmann::json &json) {
  defaultConfig();
  auto vtcs = json.at("vertices");
  vertices = {};

  // Read vertices
  for (auto &v : vtcs) {
    float x{0};
    float y{0};
    {
      auto temp = JsonUtils::getOptional<float>(v, "x");
      if (!temp) {
        // TODO: log error
        continue;
      }
      x = *temp;
    }
    {
      auto temp = JsonUtils::getOptional<float>(v, "y");
      if (!temp) {
        // TODO: log error
        continue;
      }
      y = *temp;
    }
    vertices.push_back({x, y});
  }
  // TODO: Complete
}