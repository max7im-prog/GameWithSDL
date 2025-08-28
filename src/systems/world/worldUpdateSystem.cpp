#include "worldUpdateSystem.hpp"
#include "physicsComponents.hpp"
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
        float dt = float(1.0f)/float(FPS);
        world.world->step(dt);
    }
} 