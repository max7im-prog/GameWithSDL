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
    auto v = registry.view<Creature>();
    for(auto ent:v){
        auto& comp = v.get<Creature>(ent);
        if(comp.creature){
            comp.creature->update(1.0f/FPS);
        }
    }
}