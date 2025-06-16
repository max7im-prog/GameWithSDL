#include "physicsUtils.hpp"

#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include "physicsComponents.hpp"

void PhysicsUtils::cleanupPhysicsBody(entt::registry& registry, entt::entity entity) {
    auto& physics = registry.get<PhysicsBody>(entity);
    for (b2ShapeId shape : physics.shapes) {
        b2DestroyShape(shape, false);
    }
    b2DestroyBody(physics.bodyId);
}

void PhysicsUtils::cleanupPhysicsWorld(entt::registry& registry, entt::entity entity){
    auto& world = registry.get<PhysicsWorld>(entity);
    b2DestroyWorld(world.worldId);

    auto v = registry.view<PhysicsBody>();
    for(auto& ent: v){
        auto& comp = v.get<PhysicsBody>(ent);
        if(!b2World_IsValid(comp.worldId)){
            registry.remove<PhysicsBody>(ent);
        }
    }
}
