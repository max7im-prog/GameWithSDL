#include "worldUpdateSystem.hpp"
#include "physicsObjects.hpp"
WorldUpdateSystem::WorldUpdateSystem()
{
}

WorldUpdateSystem::~WorldUpdateSystem()
{
}

void WorldUpdateSystem::update(entt::registry& registry, int FPS){
    auto view = registry.view<PhysicsWorld>();
    for(auto ent:view){
        auto& world = view.get<PhysicsWorld>(ent);
        b2World_Step(world.worldId,float(1.0f)/float(FPS),1);
    }
}