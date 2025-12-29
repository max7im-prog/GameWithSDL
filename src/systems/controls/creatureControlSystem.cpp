#include "creatureControlSystem.hpp"
#include "controlComponents.hpp"
#include "creatureComponents.hpp"
#include "gameSystem.hpp"
CreatureControlSystem::CreatureControlSystem()
    : GameSystem("CreatureControlSystem") {}

void CreatureControlSystem::update(entt::registry &registry,
                                   double secondsPassed) {
  if (!shouldRun(secondsPassed)) {
    return;
  }
  auto view = registry.view<Controller>();
  for (entt::entity ent : view) {
    auto &controller = registry.get<Controller>(ent);
    if (registry.valid(controller._creature) &&
        registry.all_of<PhysicsCreature>(controller._creature)) {

      auto &comp = registry.get<PhysicsCreature>(controller._creature);

      if (comp.creature) {
        auto &creature = comp.creature;
        creature->lookAt(controller._lookWorldPoint);
        creature->move(controller._moveDir);
        for (auto &[action, actionState]: controller._actions) {
          creature->perform(action, actionState._inputState);
        }
      }
    }
  }
}
