#include "creatureUpdateSystem.hpp"
#include "creatureComponents.hpp"


CreatureUpdateSystem::CreatureUpdateSystem()
{
}

CreatureUpdateSystem::~CreatureUpdateSystem()
{
}

void CreatureUpdateSystem::update(entt::registry& registry,int FPS)
{
    auto v = registry.view<PhysicsCreature>();
    for(auto ent:v){
        auto& comp = v.get<PhysicsCreature>(ent);
        if(comp.creature){
            comp.creature->update(1.0f/((float)FPS));
        }
    }
}