#include "creatureControlSystem.hpp"

#include "controlComponents.hpp"
#include "creatureComponents.hpp"

CreatureControlSystem::CreatureControlSystem()
{
}

CreatureControlSystem::~CreatureControlSystem()
{
}

void CreatureControlSystem::update(entt::registry &registry)
{
    // TODO: change the velocity from a hard coded value
    entt::entity controllerEnt;
    {
        auto v = registry.view<Controller>();
        if (v.size() == 0)
        {
            return;
        }
        for (auto i : v)
        {
            controllerEnt = i;
            break;
        }
    }
    auto &controller = registry.get<Controller>(controllerEnt);
    {
        auto v = registry.view<Creature>();
        for(auto [ent,cr]:v.each()){
            cr.creature->move(controller.moveDirection,0.2,0.2);
        }
    }
}
