#include "creatureFactory.hpp"
#include "connectionFactory.hpp"
#include "creatureComponents.hpp"
#include "demoCreature.hpp"
#include "physicsFactory.hpp"

CreatureFactory::CreatureFactory(
    entt::registry &registry, std::shared_ptr<World> world,
    std::shared_ptr<BodyFactory> bodyFactory,
    std::shared_ptr<ConnectionFactory> connectionFactory)
    : RegistryObjectFactory(registry), world(world), bodyFactory(bodyFactory),
      connectionFactory(connectionFactory) {}

std::shared_ptr<DemoCreature>
CreatureFactory::createDemoCreature(const DemoCreatureConfig &config) {
  return create<DemoCreature>(config);
}


