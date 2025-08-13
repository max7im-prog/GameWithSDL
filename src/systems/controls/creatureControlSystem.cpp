#include "creatureControlSystem.hpp"
#include "controlComponents.hpp"
#include "creatureComponents.hpp"

void CreatureControlSystem::update(entt::registry &registry) {
  auto view = registry.view<Controller>();
  for (entt::entity ent : view) {
    auto &controller = registry.get<Controller>(ent);
    if (registry.valid(controller.creature) &&
        registry.all_of<PhysicsCreature>(controller.creature)) {
      auto &comp = registry.get<PhysicsCreature>(controller.creature);
      if (comp.creature) {
        auto &creature = comp.creature;
        if (controller.attack) {
          creature->attack();
        }
        if (controller.defend) {
          creature->defend();
        }
        if (controller.jump) {
          creature->jump();
        }
        if (controller.aimContext.update) {
          creature->aim(controller.aimContext.worldPoint,
                        controller.aimContext.aim);
        }
        if (controller.lookContext.update) {
          creature->lookAt(controller.lookContext.worldPoint,
                           controller.lookContext.look);
        }
        if(controller.moveContext.update){
          creature->move(controller.moveContext.moveDir,controller.moveContext.moveIntensity);
        }
      }
    }
  }
}