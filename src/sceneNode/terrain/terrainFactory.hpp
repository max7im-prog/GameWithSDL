#pragma once
#include "bodyFactory.hpp"
#include "capsuleTerrain.hpp"
#include "circleTerrain.hpp"
#include "connectionFactory.hpp"
#include "miscComponents.hpp"
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
                 std::shared_ptr<BodyFactory> bodyFactory,
                 std::shared_ptr<ConnectionFactory> connectionFactory
);

  template <typename T> static constexpr bool supports() {
    return std::is_same_v<T, PolygonTerrain> ||
           std::is_same_v<T, SegmentTerrain> ||
           std::is_same_v<T, CapsuleTerrain> ||
           std::is_same_v<T, CircleTerrain>;
  }

protected:
  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent) {
    auto &comp = registry.emplace_or_replace<PhysicsTerrain>(ent);
    comp.terrain = object;
    world->addObject(std::static_pointer_cast<RegistryObject>(object));
  }

  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config) {
    auto ret = std::shared_ptr<T>(
        new T(registry, world, config, bodyFactory, connectionFactory));
    return ret;
  }
  template <typename T>
  void setUp(std::shared_ptr<T> object, const T::Config &config) {

    if (config._renderConfig) {
      registry.emplace_or_replace<EntityRequiresTexturingTag>(object->getEntity(), config._renderConfig,config._transform);
    }
  }

private:
  const std::shared_ptr<World> world;
  const std::shared_ptr<BodyFactory> bodyFactory;
  const std::shared_ptr<ConnectionFactory> connectionFactory;

  template <typename Derived> friend class RegistryObjectFactory;
};
