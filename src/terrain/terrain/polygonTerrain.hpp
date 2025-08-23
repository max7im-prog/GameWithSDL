#pragma once
#include "bodyFactory.hpp"
#include "physicsFactory.hpp"
#include "polygonBody.hpp"
#include "terrain.hpp"

struct PolygonTerrainConfig : public TerrainConfig {
  PolygonBodyConfig polygonConfig;
  static PolygonTerrainConfig defaultConfig();
};
class PolygonTerrain : public Terrain {
public:
protected:
  PolygonTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                 const PolygonTerrainConfig &config,
                 const std::shared_ptr<PhysicsFactory> physicsFactory,
                 const std::shared_ptr<BodyFactory> bodyFactory);

  std::shared_ptr<PolygonBody> polygonBody;

private:
  friend class TerrainFactory;
};