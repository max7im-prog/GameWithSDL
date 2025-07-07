#include "limbBodyPart.hpp"

#include "physicsUtils.hpp"
#include <cmath>

LimbBodyPart::LimbBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint1, b2Vec2 worldPoint2, std::vector<std::pair<float, float>> portionRadiusPairs, std::optional<b2Filter> shapeFilter) : BodyPart(registry, worldId)
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
        addJoint(pair);
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
            return b2Shape_GetCapsule(shapeArray[0]).center1;
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
            return b2Shape_GetCapsule(shapeArray[0]).center2;
        }
    }
    return {0, 0};
}

std::vector<b2Vec2> LimbBodyPart::getJointsPos()
{
    std::vector<b2Vec2> ret;
    if(this->bodies.size() == 0){
        return ret;
    }
    for(auto elem:bodies){
        ret.push_back(b2Body_GetPosition(elem.second));
    }
    ret.push_back(this->getEnd());
    return ret;
}

void LimbBodyPart::pointAt(b2Vec2 worldPoint)
{
    if (this->bodies.empty())
        return;

    constexpr int ARRAY_SIZE = 8;
    b2BodyId bodyId = this->bodies.back().second;
    size_t shapeCount = b2Body_GetShapeCount(bodyId);
    if (shapeCount == 0)
        return;

    b2ShapeId shapeArray[ARRAY_SIZE];
    b2Body_GetShapes(bodyId, shapeArray, ARRAY_SIZE);
    b2Capsule capsule = b2Shape_GetCapsule(shapeArray[0]);

    b2Vec2 worldC1 = b2Body_GetWorldPoint(bodyId, capsule.center1);
    b2Vec2 worldC2 = b2Body_GetWorldPoint(bodyId, capsule.center2);

    b2Vec2 limbDir = b2Normalize(b2Sub(worldC2, worldC1));
    b2Vec2 desiredDir = b2Normalize(b2Sub(worldPoint, worldC1));

    float angleDiff = b2Atan2(desiredDir.y, desiredDir.x) - b2Atan2(limbDir.y, limbDir.x);
    angleDiff = std::fmod(angleDiff + M_PI, 2.0f * M_PI) - M_PI;

    b2Rot curBodyRot = b2Body_GetRotation(bodyId);
    float currentAngle = b2Atan2(curBodyRot.s, curBodyRot.c);
    float desiredBodyAngle = currentAngle + angleDiff;
    desiredBodyAngle = std::fmod(desiredBodyAngle + M_PI, 2.0f * M_PI) - M_PI;

    // PD controller
    float I = b2Body_GetRotationalInertia(bodyId);
    float omega_n = 6.0f;
    float kp = I * omega_n * omega_n;
    float kd = 2.0f * I * omega_n;

    PhysicsUtils::applyTorguePD(bodyId, desiredBodyAngle, kp, kd);
}

std::pair<entt::entity, b2JointId> LimbBodyPart::getConnectionJoint()
{
    return this->connectionJoint;
}

std::pair<entt::entity, b2JointId> LimbBodyPart::connect(b2BodyId bodyId, b2Vec2 worldPoint)
{
    if(this->bodies.size() == 0){
        return {entt::null, b2JointId{}};
    }
    if(this->getConnectionJoint().first !=entt::null){
        this->registry.destroy(this->getConnectionJoint().first);
    }
    auto ent = registry.create();
    auto pair = PhysicsUtils::createRevolutePhysicsJoint(this->registry,ent,this->worldId,bodyId,this->bodies[0].second,worldPoint);
    return pair;
}
