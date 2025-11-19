#include "terrainFactory.hpp"
#include "registryObjectFactory.hpp"

TerrainFactory::TerrainFactory(
    entt::registry &registry, std::shared_ptr<World> world,
    std::shared_ptr<BodyFactory> bodyFactory,
    std::shared_ptr<ConnectionFactory> connectionFactory
)
    : RegistryObjectFactory(registry), world(world), bodyFactory(bodyFactory),
      connectionFactory(connectionFactory){}