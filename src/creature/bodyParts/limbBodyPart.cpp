#include "limbBodyPart.hpp"

#include "physicsUtils.hpp"
#include "kinematicUtils.hpp"
#include <cmath>

LimbBodyPart::LimbBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint1, b2Vec2 worldPoint2, std::vector<std::pair<float, float>> portionRadiusPairs, std::optional<b2Filter> shapeFilter) : BodyPart(registry, worldId), isTracking(false), trackingPoint({0, 0})
{
    float totalPortions = 0;
    for (auto pair : portionRadiusPairs)
    {
        totalPortions += pair.first;
    }
    for (auto pair : portionRadiusPairs)
    {
        pair.first = pair.first / totalPortions;
    }
    b2Vec2 limbDirection = b2Normalize(b2Sub(worldPoint2, worldPoint1));
    float limbLen = b2Length(b2Sub(worldPoint2, worldPoint1));

    // Creating capsules for a limb
    std::vector<std::pair<b2Vec2, b2BodyId>> createdBodies;
    b2Vec2 curPoint = worldPoint1;
    for (auto prp : portionRadiusPairs)
    {
        float portion = prp.first;
        float radius = prp.second;
        auto ent = registry.create();
        b2Vec2 increment = b2MulSV(limbLen * portion, limbDirection);
        auto pair = PhysicsUtils::createCapsulePhysicsBody(registry, ent, worldId, curPoint, {0, 0}, b2Add({0, 0}, increment), radius, shapeFilter);
        addBody(pair);
        createdBodies.push_back({curPoint, pair.second});

        curPoint = b2Add(curPoint, increment);
    }

    for (int i = 1; i < createdBodies.size(); i++)
    {
        auto ent = registry.create();
        auto pair = PhysicsUtils::createRevolutePhysicsJoint(registry, ent, worldId, createdBodies[i - 1].second, createdBodies[i].second, createdBodies[i].first);
        b2RevoluteJoint_EnableMotor(pair.second, true);
        addJoint(pair);
    }

    // Creating PID controllers for each segment
    this->PIDControllers = {};
    for (auto [ent, body] : this->bodies)
    {
        float mass = b2Body_GetMass(body);
        float omega_n = 0.2f;
        float kp = mass * omega_n * omega_n;
        float kd = 2.0f * mass * omega_n;
        float ki = 0.1f * kp;
        PIDController first(kp, ki, kd);
        PIDController second(kp, ki, kd);
        this->PIDControllers.push_back({first, second});
    }

    // Updating weight
    weightKg = 0;
    for (auto b : bodies)
    {
        weightKg += b2Body_GetMass(b.second);
    }
}

LimbBodyPart::~LimbBodyPart()
{
}

b2Vec2 LimbBodyPart::getBase()
{
    constexpr int ARRAY_SIZE = 8;
    if (this->bodies.size() != 0)
    {
        b2BodyId bodyId = this->bodies[0].second;
        b2ShapeId shapeArray[ARRAY_SIZE];
        size_t shapeCount = b2Body_GetShapeCount(bodyId);
        if (shapeCount > 0)
        {
            b2Body_GetShapes(bodyId, (b2ShapeId *)&shapeArray, ARRAY_SIZE);
            return b2Body_GetWorldPoint(bodyId, b2Shape_GetCapsule(shapeArray[0]).center1);
        }
    }
    return {0, 0};
}

b2Vec2 LimbBodyPart::getEnd()
{
    constexpr int ARRAY_SIZE = 8;
    if (this->bodies.size() != 0)
    {
        b2BodyId bodyId = this->bodies[this->bodies.size() - 1].second;
        b2ShapeId shapeArray[ARRAY_SIZE];
        size_t shapeCount = b2Body_GetShapeCount(bodyId);
        if (shapeCount > 0)
        {
            b2Body_GetShapes(bodyId, (b2ShapeId *)&shapeArray, ARRAY_SIZE);
            return b2Body_GetWorldPoint(bodyId, b2Shape_GetCapsule(shapeArray[0]).center2);
        }
    }
    return {0, 0};
}

std::vector<b2Vec2> LimbBodyPart::getJointsPos()
{
    std::vector<b2Vec2> ret;
    if (this->bodies.size() == 0)
    {
        return ret;
    }
    for (auto elem : bodies)
    {
        ret.push_back(b2Body_GetPosition(elem.second));
    }
    ret.push_back(this->getEnd());
    return ret;
}

void LimbBodyPart::trackPoint(b2Vec2 worldPoint, bool track)
{
    if (track)
    {
        this->trackingPoint = worldPoint;
        this->isTracking = true;
    }
    else
    {
        this->trackingPoint = {0, 0};
        this->isTracking = false;
        for (auto &pair : this->PIDControllers)
        {
            pair.first.reset();
            pair.second.reset();
        }
    }
}

void LimbBodyPart::updateTracking(float dt)
{
    if (!this->isTracking)
    {
        return;
    }
    if (this->bodies.empty())
        return;

    std::vector<b2Vec2> oldPos = this->getJointsPos(); // Joint positions before solving
    std::vector<b2Vec2> newPos = KinematicUtils::solveFABRIK(oldPos[0], this->trackingPoint, oldPos);

    if (newPos.size() != oldPos.size() || newPos.size() < 2)
        return;

    for (int i = 0; i < newPos.size() - 1; i++)
    {
        constexpr int ARRAY_SIZE = 10;
        float error1 = b2Distance(newPos[i], oldPos[i]);
        float error2 = b2Distance(newPos[i + 1], oldPos[i + 1]);
        float force1 = this->PIDControllers[i].first.update(error1, dt);
        float force2 = this->PIDControllers[i].second.update(error2, dt);
        b2Vec2 dir1 = b2Normalize(b2Sub(newPos[i], oldPos[i]));
        b2Vec2 dir2 = b2Normalize(b2Sub(newPos[i + 1], oldPos[i + 1]));

        b2BodyId bodyId = this->bodies[i].second;
        b2ShapeId shapeArray[ARRAY_SIZE];
        b2Body_GetShapes(bodyId, (b2ShapeId *)&shapeArray, ARRAY_SIZE);
        b2Capsule caps = b2Shape_GetCapsule(shapeArray[0]);

        b2Body_ApplyForce(bodyId, b2MulSV(force1, dir1), b2Body_GetWorldPoint(bodyId, caps.center1), true);
        b2Body_ApplyForce(bodyId, b2MulSV(force2, dir2), b2Body_GetWorldPoint(bodyId, caps.center2), true);
    }
}

std::pair<entt::entity, b2JointId> LimbBodyPart::getConnectionJoint()
{
    return this->connectionJoint;
}

std::pair<entt::entity, b2JointId> LimbBodyPart::connect(b2BodyId bodyId, b2Vec2 worldPoint)
{
    if (this->bodies.size() == 0)
    {
        return {entt::null, b2JointId{}};
    }
    if (this->getConnectionJoint().first != entt::null)
    {
        this->registry.destroy(this->getConnectionJoint().first);
    }
    auto ent = registry.create();
    auto pair = PhysicsUtils::createRevolutePhysicsJoint(this->registry, ent, this->worldId, bodyId, this->bodies[0].second, worldPoint);
    b2RevoluteJoint_EnableMotor(pair.second, true);
    this->connectionJoint = pair;
    return pair;
}

void LimbBodyPart::update(float dt)
{
    updateTracking(dt);
}
