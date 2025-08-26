#include "terrainFactory.hpp"
#include "polygonTerrain.hpp"
#include "registryObjectFactory.hpp"
#include "segmentTerrain.hpp"

TerrainFactory::TerrainFactory(entt::registry &registry,
                               std::shared_ptr<World> world,
                               std::shared_ptr<PhysicsFactory> physicsFactory,
                               std::shared_ptr<BodyFactory> bodyFactory)
    : RegistryObjectFactory(registry), world(world),
      physicsFactory(physicsFactory), bodyFactory(bodyFactory) {}

std::shared_ptr<PolygonTerrain>
TerrainFactory::createPolygonTerrain(const PolygonTerrainConfig &config) {
  return create<PolygonTerrain>(config);
}

std::shared_ptr<SegmentTerrain>
TerrainFactory::createSegmentTerrain(const SegmentTerrainConfig &config) {
  return create<SegmentTerrain>(config);
}

std::shared_ptr<CapsuleTerrain>
TerrainFactory::createCapsuleTerrain(const CapsuleTerrainConfig &config) {
  return create<CapsuleTerrain>(config);
}

std::shared_ptr<CircleTerrain>
TerrainFactory::createCircleTerrain(const CircleTerrainConfig &config) {
  return create<CircleTerrain>(config);
}