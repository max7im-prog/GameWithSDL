#include "physicsUtils.hpp"

#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include "physicsComponents.hpp"

#include <algorithm>

void PhysicsUtils::cleanupPhysicsBody(entt::registry &registry, entt::entity entity)
{
    auto &body = registry.get<PhysicsBody>(entity);
    for (b2ShapeId shape : body.shapes)
    {
        b2DestroyShape(shape, false);
    }

    // Remove all joints connected to this body from the registry
    std::vector<b2JointId> joints;
    int numJoints = b2Body_GetJointCount(body.bodyId);
    joints.resize(numJoints);
    b2Body_GetJoints(body.bodyId,joints.data(), numJoints);
    auto v = registry.view<PhysicsJoint>();
    for(auto ent:v){
        auto& comp = v.get<PhysicsJoint>(ent);
        for(auto jointId:joints){
            if(B2_ID_EQUALS(jointId,comp.jointId)){
                registry.remove<PhysicsJoint>(ent);
                break;
            }
        }
    }

    if(b2Body_IsValid(body.bodyId))b2DestroyBody(body.bodyId);
}

void PhysicsUtils::cleanupPhysicsWorld(entt::registry &registry, entt::entity entity)
{
    auto &world = registry.get<PhysicsWorld>(entity);
    b2DestroyWorld(world.worldId);

    auto bodies = registry.view<PhysicsBody>();
    for (auto &ent : bodies)
    {
        auto &comp = bodies.get<PhysicsBody>(ent);
        if (!b2World_IsValid(comp.worldId))
        {
            registry.remove<PhysicsBody>(ent);
        }
    }

    auto joints = registry.view<PhysicsJoint>();
    for (auto &ent : joints)
    {
        auto &comp = joints.get<PhysicsJoint>(ent);
        if (!b2World_IsValid(comp.worldId))
        {
            registry.remove<PhysicsJoint>(ent);
        }
    }

    
}

void PhysicsUtils::cleanupPhysicsJoint(entt::registry &registry, entt::entity entity)
{
    auto& joint = registry.get<PhysicsJoint>(entity);
    if(b2Joint_IsValid(joint.jointId))b2DestroyJoint(joint.jointId);
}

void PhysicsUtils::createPolygonPhysicsBody(entt::registry &registry, const entt::entity &entity, b2WorldId worldId, b2Vec2 position, const std::vector<b2Vec2> vertices, b2BodyType bodyType, float density, float friction, float restitution)
{
    auto bodyDef = b2DefaultBodyDef();
    bodyDef.type = bodyType;
    bodyDef.position = position;
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    b2Hull hull = b2ComputeHull(vertices.data(), std::min(int(vertices.size()), B2_MAX_POLYGON_VERTICES));
    b2Polygon polygon = b2MakePolygon(&hull, 0);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;
    shapeDef.material.restitution = restitution;
    b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &polygon);

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
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    b2Circle circle;
    circle.center = center;
    circle.radius = radius;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;
    shapeDef.material.restitution = restitution;
    b2ShapeId shapeId = b2CreateCircleShape(bodyId, &shapeDef, &circle);

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
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    b2Capsule capsule;
    capsule.center1 = center1;
    capsule.center2 = center2;
    capsule.radius = radius;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;
    shapeDef.material.restitution = restitution;
    b2ShapeId shapeId = b2CreateCapsuleShape(bodyId, &shapeDef, &capsule);

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
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    b2Segment segment;
    segment.point1 = point1;
    segment.point2 = point2;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;
    shapeDef.material.restitution = restitution;
    b2ShapeId shapeId = b2CreateSegmentShape(bodyId, &shapeDef, &segment);

    auto &comp = registry.emplace_or_replace<PhysicsBody>(entity);
    comp.bodyId = bodyId;
    comp.worldId = worldId;
    comp.shapes = {shapeId};
}

void PhysicsUtils::createDistancePhysicsJoint(entt::registry &registry, const entt::entity &entity, b2WorldId worldId, b2BodyId bodyAId, b2BodyId bodyBId, b2Vec2 localPointA, b2Vec2 localPointB, std::optional<float> length, bool isSpring, float freq, float damping)
{
    b2DistanceJointDef jointDef = b2DefaultDistanceJointDef();
    jointDef.base.bodyIdA = bodyAId;
    jointDef.base.bodyIdB = bodyBId;

    jointDef.base.localFrameA.p = localPointA;
    jointDef.base.localFrameA.q = b2MakeRot(0);
    jointDef.base.localFrameB.p = localPointB;
    jointDef.base.localFrameB.q = b2MakeRot(0);

    if (isSpring)
    {
        jointDef.enableSpring = true;
        jointDef.hertz = freq;
        jointDef.dampingRatio = damping;
    }

    if (length.has_value())
    {
        jointDef.length = length.value();
    }
    else
    {
        float distance = b2Distance(b2Body_GetWorldPoint(bodyAId, localPointA), b2Body_GetWorldPoint(bodyBId, localPointB));
        jointDef.length = distance;
    }

    b2JointId jointId = b2CreateDistanceJoint(worldId, &jointDef);

    auto &comp = registry.emplace_or_replace<PhysicsJoint>(entity);
    comp.bodyAId = bodyAId;
    comp.bodyBId = bodyBId;
    comp.jointId = jointId;
    comp.worldId = worldId;
}

void PhysicsUtils::createRevolutePhysicsJoint(entt::registry &registry, const entt::entity &entity, b2WorldId worldId, b2BodyId bodyAId, b2BodyId bodyBId, b2Vec2 worldPoint)
{
    b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef();
    jointDef.base.bodyIdA = bodyAId;
    jointDef.base.bodyIdB = bodyBId;

    jointDef.base.localFrameA.p = b2Body_GetLocalPoint(bodyAId,worldPoint);
    jointDef.base.localFrameA.q = b2MakeRot(0);
    jointDef.base.localFrameB.p = b2Body_GetLocalPoint(bodyBId,worldPoint);
    jointDef.base.localFrameB.q = b2MakeRot(0);

    b2JointId jointId = b2CreateRevoluteJoint(worldId, &jointDef);

    auto &comp = registry.emplace_or_replace<PhysicsJoint>(entity);
    comp.bodyAId = bodyAId;
    comp.bodyBId = bodyBId;
    comp.jointId = jointId;
    comp.worldId = worldId;
}
