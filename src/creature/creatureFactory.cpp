#include "creatureFactory.hpp"
#include "creatureComponents.hpp"
#include "physicsFactory.hpp"

CreatureFactory::CreatureFactory(entt::registry &registry,
                                 std::shared_ptr<World> world,
                                 std::shared_ptr<PhysicsFactory> physicsFactory,
                                 std::shared_ptr<BodyFactory> bodyFactory)
    : registry(registry), world(world), physicsFactory(physicsFactory),
      bodyFactory(bodyFactory) {}

std::shared_ptr<DemoCreature>
CreatureFactory::createDemoCreature(const DemoCreatureConfig &config) {
  std::shared_ptr<DemoCreature> ret = nullptr;
  try {
    ret = std::shared_ptr<DemoCreature>(
        new DemoCreature(registry, world, config, physicsFactory, bodyFactory));
  } catch (std::exception &e) {
    return nullptr;
  }
  ret->entity = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsCreature>(ret->entity);
  comp.creature = ret;
  return ret;
}