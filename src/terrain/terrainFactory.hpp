#pragma once
#include "bodyFactory.hpp"
#include "physicsFactory.hpp"
#include "polygonTerrain.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
#include <memory>
class TerrainFactory {
public:
  TerrainFactory(entt::registry &registry, std::shared_ptr<World> world,
                 std::shared_ptr<PhysicsFactory> physicsFactory,
                 std::shared_ptr<BodyFactory> bodyFactory);


  std::shared_ptr<PolygonTerrain> createPolygonTerrain(const PolygonTerrainConfig& config);

private:
  TerrainFactory() = delete;
  TerrainFactory(TerrainFactory &other) = delete;
  TerrainFactory(TerrainFactory &&other) = delete;
  TerrainFactory &operator=(TerrainFactory &other) = delete;
  TerrainFactory &operator=(TerrainFactory &&other) = delete;

  entt::registry &registry;
  const std::shared_ptr<World> world;
  const std::shared_ptr<PhysicsFactory> physicsFactory;
  const std::shared_ptr<BodyFactory> bodyFactory;
};