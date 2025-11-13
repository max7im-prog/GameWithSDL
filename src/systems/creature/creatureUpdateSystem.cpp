#include "creatureUpdateSystem.hpp"
#include "creatureComponents.hpp"
#include "gameSystem.hpp"

CreatureUpdateSystem::CreatureUpdateSystem()
    : GameSystem("CreatureUpdateSystem") {}

CreatureUpdateSystem::~CreatureUpdateSystem() {}

void CreatureUpdateSystem::update(entt::registry &registry,
                                  double secondsPassed) {

  if (!shouldRun(secondsPassed)) {
    return;
  }
  auto v = registry.view<PhysicsCreature>();
  for (auto ent : v) {
    auto &comp = v.get<PhysicsCreature>(ent);
    if (comp.creature) {
      comp.creature->update(secondsPassed);
    }
  }
}