#pragma once
#include "bodyFactory.hpp"
#include "physicsFactory.hpp"
#include "polygonTerrain.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
#include <memory>
class TerrainFactory: public RegistryObjectFactory {
public:
  TerrainFactory(entt::registry &registry, std::shared_ptr<World> world,
                 std::shared_ptr<PhysicsFactory> physicsFactory,
                 std::shared_ptr<BodyFactory> bodyFactory);


  std::shared_ptr<PolygonTerrain> createPolygonTerrain(const PolygonTerrainConfig& config);

private:
  void registerTerrain(std::shared_ptr<Terrain> terrain);

  const std::shared_ptr<World> world;
  const std::shared_ptr<PhysicsFactory> physicsFactory;
  const std::shared_ptr<BodyFactory> bodyFactory;
};