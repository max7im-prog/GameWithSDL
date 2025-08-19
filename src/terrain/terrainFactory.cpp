#include "terrainFactory.hpp"
#include "terrainComponents.hpp"

TerrainFactory::TerrainFactory(entt::registry &registry,
                               std::shared_ptr<World> world,
                               std::shared_ptr<PhysicsFactory> physicsFactory,
                               std::shared_ptr<BodyFactory> bodyFactory)
    : registry(registry), world(world), physicsFactory(physicsFactory),
      bodyFactory(bodyFactory) {}

std::shared_ptr<PolygonTerrain>
TerrainFactory::createPolygonTerrain(const PolygonTerrainConfig &config) {
  std::shared_ptr<PolygonTerrain> ret = nullptr;
  try {
    ret = std::shared_ptr<PolygonTerrain>(
        new PolygonTerrain(registry, world, config, physicsFactory, bodyFactory));
  } catch (std::exception &e) {
    return nullptr;
  }
  ret->entity = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsTerrain>(ret->entity);
  comp.terrain = ret;
  return ret;
}