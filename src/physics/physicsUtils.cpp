#include "physicsUtils.hpp"

#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include "physicsComponents.hpp"

#include <algorithm>

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

void PhysicsUtils::createPolygonPhysicsShape(entt::registry &registry, const entt::entity & entity,b2WorldId worldId, b2Vec2 position,const std::vector<b2Vec2> vertices,  b2BodyType bodyType, float density, float friction,float restitution)
{

    auto bodyDef = b2DefaultBodyDef();
    bodyDef.type = bodyType;
    bodyDef.position = position;
    b2BodyId bodyId = b2CreateBody(worldId,&bodyDef);

    b2Hull hull = b2ComputeHull(vertices.data(),std::min(int(vertices.size()),B2_MAX_POLYGON_VERTICES));
    b2Polygon polygon = b2MakePolygon(&hull,0);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;
    shapeDef.material.restitution = restitution;
    b2ShapeId shapeId = b2CreatePolygonShape(bodyId,&shapeDef,&polygon);

    auto &comp = registry.emplace_or_replace<PhysicsBody>(entity);
    comp.bodyId = bodyId;
    comp.worldId = worldId;
    comp.shapes = {shapeId};
}
