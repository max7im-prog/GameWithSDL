#include "creatureFactory.hpp"
#include "connectionFactory.hpp"

CreatureFactory::CreatureFactory(
    entt::registry &registry, std::shared_ptr<World> world,
    std::shared_ptr<BodyFactory> bodyFactory,
    std::shared_ptr<ConnectionFactory> connectionFactory)
    : RegistryObjectFactory(registry), world(world), bodyFactory(bodyFactory),
      connectionFactory(connectionFactory) {}
