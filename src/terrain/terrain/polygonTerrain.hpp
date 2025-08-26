#pragma once
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "connectionFactory.hpp"
#include "polygonBody.hpp"
#include "terrain.hpp"

struct PolygonTerrainConfig : public TerrainConfig {
  PolygonBodyConfig templateBodyCfg;
  std::vector<b2Vec2> vertices;
  static PolygonTerrainConfig defaultConfig();
};
class PolygonTerrain : public Terrain {
public:
  using Config = PolygonTerrainConfig;

protected:
  PolygonTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                 const PolygonTerrainConfig &config,
                 const std::shared_ptr<BodyFactory> bodyFactory,
                 const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::shared_ptr<PolygonBody> polygonBody;

private:
  friend class TerrainFactory;
};