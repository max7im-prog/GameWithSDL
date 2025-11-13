#include "worldUpdateSystem.hpp"
#include "physicsComponents.hpp"
WorldUpdateSystem::WorldUpdateSystem() : GameSystem("WorldUpdateSystem") {}

WorldUpdateSystem::~WorldUpdateSystem() {}

void WorldUpdateSystem::update(entt::registry &registry, double secondsPassed) {
  if (!shouldRun(secondsPassed)) {
    return;
  }
  auto view = registry.view<PhysicsWorld>();
  for (auto ent : view) {
    auto &world = view.get<PhysicsWorld>(ent);
    world.world->step(secondsPassed);
  }
}