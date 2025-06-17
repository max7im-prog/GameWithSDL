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

void PhysicsUtils::createPolygonPhysicsBody(entt::registry &registry, const entt::entity & entity,b2WorldId worldId, b2Vec2 position,const std::vector<b2Vec2> vertices,  b2BodyType bodyType, float density, float friction,float restitution)
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

void PhysicsUtils::createCirclePhysicsBody(entt::registry &registry, const entt::entity &entity, b2WorldId worldId, b2Vec2 position, b2Vec2 center, float radius, b2BodyType bodyType, float density, float friction, float restitution)
{
    auto bodyDef = b2DefaultBodyDef();
    bodyDef.type = bodyType;
    bodyDef.position = position;
    b2BodyId bodyId = b2CreateBody(worldId,&bodyDef);

    b2Circle circle;
    circle.center = center;
    circle.radius = radius;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;
    shapeDef.material.restitution = restitution;
    b2ShapeId shapeId = b2CreateCircleShape(bodyId,&shapeDef, &circle);

    auto &comp = registry.emplace_or_replace<PhysicsBody>(entity);
    comp.bodyId = bodyId;
    comp.worldId = worldId;
    comp.shapes = {shapeId};
}

void PhysicsUtils::createCapsulePhysicsBody(entt::registry &registry, const entt::entity &entity, b2WorldId worldId, b2Vec2 position, b2Vec2 center1, b2Vec2 center2, float radius, b2BodyType bodyType, float density, float friction, float restitution)
{
    auto bodyDef = b2DefaultBodyDef();
    bodyDef.type = bodyType;
    bodyDef.position = position;
    b2BodyId bodyId = b2CreateBody(worldId,&bodyDef);

    b2Capsule capsule;
    capsule.center1 = center1;
    capsule.center2 = center2;
    capsule.radius = radius;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;
    shapeDef.material.restitution = restitution;
    b2ShapeId shapeId = b2CreateCapsuleShape(bodyId,&shapeDef,&capsule);

    auto &comp = registry.emplace_or_replace<PhysicsBody>(entity);
    comp.bodyId = bodyId;
    comp.worldId = worldId;
    comp.shapes = {shapeId};
}

void PhysicsUtils::createSegmentPhysicsBody(entt::registry &registry, const entt::entity &entity, b2WorldId worldId, b2Vec2 position, b2Vec2 point1, b2Vec2 point2, b2BodyType bodyType, float density, float friction, float restitution)
{
    auto bodyDef = b2DefaultBodyDef();
    bodyDef.type = bodyType;
    bodyDef.position = position;
    b2BodyId bodyId = b2CreateBody(worldId,&bodyDef);

    b2Segment segment;
    segment.point1 = point1;
    segment.point2 = point2;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;
    shapeDef.material.restitution = restitution;
    b2ShapeId shapeId = b2CreateSegmentShape(bodyId,&shapeDef,&segment);

    auto &comp = registry.emplace_or_replace<PhysicsBody>(entity);
    comp.bodyId = bodyId;
    comp.worldId = worldId;
    comp.shapes = {shapeId};
}
