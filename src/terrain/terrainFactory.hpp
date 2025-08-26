#pragma once
#include "bodyFactory.hpp"
#include "capsuleTerrain.hpp"
#include "circleTerrain.hpp"
#include "physicsFactory.hpp"
#include "polygonTerrain.hpp"
#include "registryObjectFactory.hpp"
#include "segmentTerrain.hpp"
#include "terrainComponents.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
#include <memory>
class TerrainFactory : public RegistryObjectFactory<TerrainFactory> {
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
  std::shared_ptr<CircleTerrain>
  createCircleTerrain(const CircleTerrainConfig &config);

protected:
  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent) {
    auto &comp = registry.emplace_or_replace<PhysicsTerrain>(ent);
    comp.terrain = object;
  }

  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config) {
    return std::shared_ptr<T>(
        new T(registry, world, config, physicsFactory, bodyFactory));
  }

private:
  const std::shared_ptr<World> world;
  const std::shared_ptr<PhysicsFactory> physicsFactory;
  const std::shared_ptr<BodyFactory> bodyFactory;

  template <typename Derived> friend class RegistryObjectFactory;
};