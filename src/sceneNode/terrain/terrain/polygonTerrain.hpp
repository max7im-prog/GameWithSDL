#pragma once
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "connectionFactory.hpp"
#include "polygonBody.hpp"
#include "terrain.hpp"

struct PolygonTerrainConfig : public TerrainConfig {
  PolygonBodyConfig templateBodyCfg;
  std::vector<b2Vec2> vertices;
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;
};
class PolygonTerrain : public Terrain , public VisitableImpl<PolygonTerrain>{
public:
  using Config = PolygonTerrainConfig;
  virtual b2Vec2 getWorldPos() override;

protected:
  PolygonTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                 const PolygonTerrainConfig &config,
                 const std::shared_ptr<BodyFactory> bodyFactory,
                 const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::weak_ptr<PolygonBody> polygonBody;

private:
  friend class TerrainFactory;
};