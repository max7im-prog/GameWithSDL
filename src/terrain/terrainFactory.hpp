#pragma once
#include "bodyFactory.hpp"
#include "capsuleTerrain.hpp"
#include "physicsFactory.hpp"
#include "polygonTerrain.hpp"
#include "segmentTerrain.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
#include <memory>
class TerrainFactory : public RegistryObjectFactory {
public:
  TerrainFactory(entt::registry &registry, std::shared_ptr<World> world,
                 std::shared_ptr<PhysicsFactory> physicsFactory,
                 std::shared_ptr<BodyFactory> bodyFactory);

  std::shared_ptr<PolygonTerrain>
  createPolygonTerrain(const PolygonTerrainConfig &config);
  std::shared_ptr<SegmentTerrain>
  createSegmentTerrain(const SegmentTerrainConfig &config);
  std::shared_ptr<CapsuleTerrain>
  createCapsuleTerrain(const CapsuleTerrainConfig &config);

private:
  void registerTerrain(std::shared_ptr<Terrain> terrain);

  const std::shared_ptr<World> world;
  const std::shared_ptr<PhysicsFactory> physicsFactory;
  const std::shared_ptr<BodyFactory> bodyFactory;
};