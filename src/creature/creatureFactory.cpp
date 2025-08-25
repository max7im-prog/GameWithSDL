#include "creatureFactory.hpp"
#include "connectionFactory.hpp"
#include "creatureComponents.hpp"
#include "physicsFactory.hpp"

CreatureFactory::CreatureFactory(
    entt::registry &registry, std::shared_ptr<World> world,
    std::shared_ptr<BodyFactory> bodyFactory,
    std::shared_ptr<ConnectionFactory> connectionFactory)
    : RegistryObjectFactory(registry), world(world), bodyFactory(bodyFactory),
      connectionFactory(connectionFactory) {}

std::shared_ptr<DemoCreature>
CreatureFactory::createDemoCreature(const DemoCreatureConfig &config) {
  std::shared_ptr<DemoCreature> ret = nullptr;
  try {
    ret = std::shared_ptr<DemoCreature>(new DemoCreature(
        registry, world, config, bodyFactory, connectionFactory));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerCreature(ret);
  return ret;
}

void CreatureFactory::registerCreature(std::shared_ptr<Creature> creature) {
  auto ent = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsCreature>(ent);
  comp.creature = creature;
  creature->setEntity(ent);
}