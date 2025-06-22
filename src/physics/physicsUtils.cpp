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
        if(b2Shape_IsValid(shape))
            b2DestroyShape(shape, false);
    }

    // // Remove all joints connected to this body from the registry
    // std::vector<b2JointId> joints;
    // int numJoints = b2Body_GetJointCount(body.bodyId);
    // joints.resize(numJoints);
    // b2Body_GetJoints(body.bodyId, joints.data(), numJoints);
    // auto v = registry.view<PhysicsJoint>();
    // for (auto ent : v)
    // {
    //     auto &comp = v.get<PhysicsJoint>(ent);
    //     for (auto jointId : joints)
    //     {
    //         if (B2_ID_EQUALS(jointId, comp.jointId))
    //         {
    //             registry.remove<PhysicsJoint>(ent);
    //             break;
    //         }
    //     }
    // }

    if (b2Body_IsValid(body.bodyId))
        b2DestroyBody(body.bodyId);
}

void PhysicsUtils::cleanupPhysicsWorld(entt::registry &registry, entt::entity entity)
{
    auto &world = registry.get<PhysicsWorld>(entity);

    // auto bodies = registry.view<PhysicsBody>();
    // for (auto ent : bodies)
    // {
    //     auto &comp = bodies.get<PhysicsBody>(ent);
    //     if (comp.worldId.index1 == world.worldId.index1 && comp.worldId.generation == world.worldId.generation)
    //     {
    //         registry.remove<PhysicsBody>(ent);
    //     }
    // }

    // auto joints = registry.view<PhysicsJoint>();
    // for (auto &ent : joints)
    // {
    //     auto &comp = joints.get<PhysicsJoint>(ent);
    //     if (comp.worldId.index1 == world.worldId.index1 && comp.worldId.generation == world.worldId.generation)
    //     {
    //         registry.remove<PhysicsJoint>(ent);
    //     }
    // }

    if (b2World_IsValid(world.worldId))
        b2DestroyWorld(world.worldId);
}

void PhysicsUtils::cleanupPhysicsJoint(entt::registry &registry, entt::entity entity)
{
    auto &joint = registry.get<PhysicsJoint>(entity);
    if (b2Joint_IsValid(joint.jointId))
        b2DestroyJoint(joint.jointId);
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
    jointDef.bodyIdA = bodyAId;
    jointDef.bodyIdB = bodyBId;

    jointDef.localAnchorA = localPointA;
    // jointDef.base.localFrameA.q = b2MakeRot(0);
    jointDef.localAnchorB = localPointB;
    // jointDef.base.localFrameB.q = b2MakeRot(0);

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
        if (distance < 0.01f)
        {
            distance = 0.01f;
        }
        jointDef.length = distance;
    }

    b2JointId jointId = b2CreateDistanceJoint(worldId, &jointDef);

    auto &comp = registry.emplace_or_replace<PhysicsJoint>(entity);
    comp.bodyAId = bodyAId;
    comp.bodyBId = bodyBId;
    comp.jointId = jointId;
    comp.worldId = worldId;
}

void PhysicsUtils::createEmptyPhysicsBody(entt::registry &registry, const entt::entity &entity, b2WorldId worldId, b2Vec2 position, b2BodyType bodyType)
{
    auto bodyDef = b2DefaultBodyDef();
    bodyDef.type = bodyType;
    bodyDef.position = position;
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    auto &comp = registry.emplace_or_replace<PhysicsBody>(entity);
    comp.bodyId = bodyId;
    comp.worldId = worldId;
    comp.shapes = {};
}

void PhysicsUtils::createRevolutePhysicsJoint(entt::registry &registry, const entt::entity &entity, b2WorldId worldId, b2BodyId bodyAId, b2BodyId bodyBId, b2Vec2 worldPoint)
{
    b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef();
    jointDef.bodyIdA = bodyAId;
    jointDef.bodyIdB = bodyBId;

    jointDef.localAnchorA = b2Body_GetLocalPoint(bodyAId, worldPoint);

    jointDef.localAnchorB = b2Body_GetLocalPoint(bodyBId, worldPoint);

    b2JointId jointId = b2CreateRevoluteJoint(worldId, &jointDef);

    auto &comp = registry.emplace_or_replace<PhysicsJoint>(entity);
    comp.bodyAId = bodyAId;
    comp.bodyBId = bodyBId;
    comp.jointId = jointId;
    comp.worldId = worldId;
}

void PhysicsUtils::createMousePhysicsJoint(entt::registry &registry, const entt::entity &entity, b2WorldId worldId, b2BodyId mouseBodyId, b2BodyId targetBodyId, b2Vec2 mouseLocalPoint, b2Vec2 targetLocalPoint)
{
    b2MouseJointDef jointDef = b2DefaultMouseJointDef();
    jointDef.bodyIdA = mouseBodyId;
    jointDef.bodyIdB = targetBodyId;

    jointDef.target = mouseLocalPoint;
    // jointDef.base.localFrameA.q = b2MakeRot(0);
    // jointDef.base.localFrameB.p = targetLocalPoint;
    // jointDef.base.localFrameB.q = b2MakeRot(0);

    jointDef.maxForce = 1000 * b2Body_GetMass(targetBodyId) + 1000;
    jointDef.dampingRatio = 50;
    jointDef.hertz = 1000;

    b2JointId jointId = b2CreateMouseJoint(worldId, &jointDef);

    auto &comp = registry.emplace_or_replace<PhysicsJoint>(entity);
    comp.bodyAId = mouseBodyId;
    comp.bodyBId = targetBodyId;
    comp.jointId = jointId;
    comp.worldId = worldId;
}

std::vector<b2ShapeId> PhysicsUtils::getShapeAtPosition(b2WorldId worldId, b2Vec2 position)
{
    std::vector<b2ShapeId> potential = {};
    std::vector<b2ShapeId> ret = {};

    b2ShapeProxy proxy;
    proxy.count = 1;
    proxy.points[0] = position;
    proxy.radius = 0;

    const float epsilon = 0.001f;
    b2AABB aabb;
    aabb.lowerBound = {position.x - epsilon, position.y - epsilon};
    aabb.upperBound = {position.x + epsilon, position.y + epsilon};

    // TODO: find a proper way to initialize
    b2QueryFilter filter;
    filter.maskBits = 1 | 2 | 4 | 8 | 16 | 32;
    filter.categoryBits = 1 | 2 | 4 | 8 | 16 | 32;

    b2World_OverlapAABB(worldId, aabb, filter, &PhysicsUtils::pointOverlapCallbackFunction, &potential);

    for (auto sh : potential)
    {
        if (b2Shape_TestPoint(sh, position))
        {
            ret.push_back(sh);
        }
    }
    return ret;
}

b2BodyId PhysicsUtils::getBodyId(entt::registry &registry, entt::entity ent)
{
    b2BodyId ret = b2_nullBodyId;

    if (registry.all_of<PhysicsBody>(ent))
    {
        auto &phBody = registry.get<PhysicsBody>(ent);
        ret = phBody.bodyId;
    }

    return ret;
}

int PhysicsUtils::getNextNegativeId()
{
    static int ret = -1;
    return ret--;
}

int PhysicsUtils::getNextPositiveId()
{
    static int ret = 1;
    return ret++;
}

bool PhysicsUtils::pointOverlapCallbackFunction(b2ShapeId shapeId, void *context)
{
    std::vector<b2ShapeId> *shapes = static_cast<std::vector<b2ShapeId> *>(context);
    shapes->push_back(shapeId);
    return true;
}
