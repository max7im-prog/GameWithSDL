#pragma once
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "connectionFactory.hpp"
#include "polygonBody.hpp"
#include "terrain.hpp"

class PolygonTerrain : public Terrain, public VisitableImpl<PolygonTerrain> {
public:
  struct Config : public Terrain::Config {
    PolygonBody::Config templateBodyCfg;
    std::vector<b2Vec2> vertices;
    void defaultConfig() override;
    void fromJSON(const nlohmann::json &json) override;
  };
  virtual b2Vec2 getWorldPos() override;

protected:
  PolygonTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                 const PolygonTerrain::Config &config,
                 const std::shared_ptr<BodyFactory> bodyFactory,
                 const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::weak_ptr<PolygonBody> polygonBody;

private:
  friend class TerrainFactory;
};
