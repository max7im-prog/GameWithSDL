#include "humanoid.hpp"
#include "physicsUtils.hpp"
#include "baseCreature.hpp"
#include "physicsComponents.hpp"

Humanoid::Humanoid(entt::registry &registry,
                   entt::entity self,
                   b2WorldId worldId,
                   b2Vec2 position,
                   float sizeXMeters,
                   float sizeYMeters) : BaseCreature(registry, self, worldId, position, PhysicsUtils::getNextNegativeId()), sizeXMeters(sizeXMeters), sizeYMeters(sizeYMeters)
{
    float measureX = sizeXMeters / 17.0f; // I got the proportions from a drawing of a ragdoll
    float measureY = sizeYMeters / 14.5f;
    float measure = std::min(measureX, measureY);

    legHeight = measureY*6.0f + 0.5f*measure;

    b2Vec2 posNeckBase = b2Add(position, {0, 0});
    b2Vec2 posHeadBase = b2Add(posNeckBase, {0, measureY * 1.0f});
    b2Vec2 posTorsoBase = b2Add(posNeckBase, {0, -5.0f * measureY});
    b2Vec2 posLeftShoulder = b2Add(posNeckBase, {-2.0f * measureX, 0});
    b2Vec2 posRightShoulder = b2Add(posNeckBase, {2.0f * measureX, 0});
    b2Vec2 posLeftElbow = b2Add(posLeftShoulder, {-3.0f * measureX, 0});
    b2Vec2 posRightElbow = b2Add(posRightShoulder, {3.0f * measureX, 0});
    b2Vec2 posLeftPalm = b2Add(posLeftElbow, {-3.0f * measureX, 0});
    b2Vec2 posRightPalm = b2Add(posRightElbow, {3.0f * measureX, 0});
    b2Vec2 posLeftHip = b2Add(posTorsoBase, {-0.75f * measureX, 0});
    b2Vec2 posRightHip = b2Add(posTorsoBase, {0.75f * measureX, 0});
    b2Vec2 posLeftKnee = b2Add(posLeftHip, {0, -3.0f * measureY});
    b2Vec2 posRightKnee = b2Add(posRightHip, {0, -3.0f * measureY});
    b2Vec2 posLeftFoot = b2Add(posLeftKnee, {0, -3.0f * measureY});
    b2Vec2 posRightFoot = b2Add(posRightKnee, {0, -3.0f * measureY});

    // Filter to disable collisions between the body parts
    b2Filter filter = b2DefaultFilter();
    filter.groupIndex = this->groupId;

    // Neck and torso
    neck = createBodyEntity();
    PhysicsUtils::createCapsulePhysicsBody(this->registry, neck, worldId, posNeckBase, {0, 0}, b2Sub(posHeadBase, position), 0.5f * measure, filter);
    head = createBodyEntity();
    std::vector<b2Vec2> headShape = {{-1.0f * measureX, 0}, {-1.0f * measureX, 2.0f * measureY}, {1.0f * measureX, 2.0f * measureY}, {1.0f * measureX, 0}};
    PhysicsUtils::createPolygonPhysicsBody(this->registry, head, worldId, posHeadBase, headShape, filter);
    torso = createBodyEntity();
    std::vector<b2Vec2> torsoShape = {{0, 0}, {-2.0f * measureX, 5.0f * measureY}, {2.0f * measureX, 5.0f * measureY}};
    PhysicsUtils::createPolygonPhysicsBody(this->registry, torso, worldId, posTorsoBase, torsoShape, filter);
    b2Body_SetLinearDamping(PhysicsUtils::getBodyId(this->registry,torso),1.0f);

    // Limb parts
    upperArmLeft = createBodyEntity();
    PhysicsUtils::createCapsulePhysicsBody(this->registry, upperArmLeft, worldId, posLeftShoulder, {0, 0}, {-3.0f * measureX, 0}, 0.5F * measure, filter);
    upperArmRight = createBodyEntity();
    PhysicsUtils::createCapsulePhysicsBody(this->registry, upperArmRight, worldId, posRightShoulder, {0, 0}, {3.0f * measureX, 0}, 0.5F * measure, filter);
    forearmLeft = this->registry.create();
    PhysicsUtils::createCapsulePhysicsBody(this->registry, forearmLeft, worldId, posLeftElbow, {0, 0}, {-3.0f * measureX, 0}, 0.5F * measure, filter);
    this->bodies.push_back(forearmLeft);
    forearmRight = createBodyEntity();
    PhysicsUtils::createCapsulePhysicsBody(this->registry, forearmRight, worldId, posRightElbow, {0, 0}, {3.0f * measureX, 0}, 0.5F * measure, filter);
    femurLeft = createBodyEntity();
    PhysicsUtils::createCapsulePhysicsBody(this->registry, femurLeft, worldId, posLeftHip, {0, 0}, {0, -3.0f * measureY}, 0.5F * measure, filter);
    femurRight = createBodyEntity();
    PhysicsUtils::createCapsulePhysicsBody(this->registry, femurRight, worldId, posRightHip, {0, 0}, {0, -3.0f * measureY}, 0.5F * measure, filter);
    calfLeft = createBodyEntity();
    PhysicsUtils::createCapsulePhysicsBody(this->registry, calfLeft, worldId, posLeftKnee, {0, 0}, {0, -3.0f * measureY}, 0.5F * measure, filter);
    calfRight = createBodyEntity();
    PhysicsUtils::createCapsulePhysicsBody(this->registry, calfRight, worldId, posRightKnee, {0, 0}, {0, -3.0f * measureY}, 0.5F * measure, filter);

    this->updateWeight();

    // Connect limbs
    b2JointId temp;
    temp = connectRevolute(neck, head, posHeadBase);
    // b2RevoluteJoint_SetLimits(temp,-0.1f,0.1f);
    // b2RevoluteJoint_EnableLimit(temp, true);

    temp = connectRevolute(neck, torso, posNeckBase);
    b2RevoluteJoint_SetLimits(temp, -0.1f, 0.1f);
    b2RevoluteJoint_EnableLimit(temp, true);
    

    temp = connectRevolute(torso, upperArmLeft, posLeftShoulder);
    b2RevoluteJoint_EnableMotor(temp,true);
    b2RevoluteJoint_SetMaxMotorTorque(temp,b2Body_GetMass(PhysicsUtils::getBodyId(this->registry,upperArmLeft))/11);
    b2RevoluteJoint_SetMotorSpeed(temp,0.0f);

    temp = connectRevolute(torso, upperArmRight, posRightShoulder);
    b2RevoluteJoint_EnableMotor(temp,true);
    b2RevoluteJoint_SetMaxMotorTorque(temp,b2Body_GetMass(PhysicsUtils::getBodyId(this->registry,upperArmRight))/11);
    b2RevoluteJoint_SetMotorSpeed(temp,0.0f);

    temp = connectRevolute(torso, femurLeft, posLeftHip);
    b2RevoluteJoint_EnableMotor(temp,true);
    b2RevoluteJoint_SetMaxMotorTorque(temp,b2Body_GetMass(PhysicsUtils::getBodyId(this->registry,femurLeft))/15);
    b2RevoluteJoint_SetMotorSpeed(temp,0.0f);

    temp = connectRevolute(torso, femurRight, posRightHip);
    b2RevoluteJoint_EnableMotor(temp,true);
    b2RevoluteJoint_SetMaxMotorTorque(temp,b2Body_GetMass(PhysicsUtils::getBodyId(this->registry,femurRight))/15);
    b2RevoluteJoint_SetMotorSpeed(temp,0.0f);

    temp = connectRevolute(femurLeft, calfLeft, posLeftKnee);
    b2RevoluteJoint_EnableMotor(temp,true);
    b2RevoluteJoint_SetMaxMotorTorque(temp,b2Body_GetMass(PhysicsUtils::getBodyId(this->registry,calfLeft))/15);
    b2RevoluteJoint_SetMotorSpeed(temp,0.0f);


    temp = connectRevolute(femurRight, calfRight, posRightKnee);
    b2RevoluteJoint_EnableMotor(temp,true);
    b2RevoluteJoint_SetMaxMotorTorque(temp,b2Body_GetMass(PhysicsUtils::getBodyId(this->registry,calfRight))/15);
    b2RevoluteJoint_SetMotorSpeed(temp,0.0f);

    temp = connectRevolute(forearmLeft, upperArmLeft, posLeftElbow);
    b2RevoluteJoint_EnableMotor(temp,true);
    b2RevoluteJoint_SetMaxMotorTorque(temp,b2Body_GetMass(PhysicsUtils::getBodyId(this->registry,forearmLeft))/15);
    b2RevoluteJoint_SetMotorSpeed(temp,0.0f);

    temp = connectRevolute(forearmRight, upperArmRight, posRightElbow);
    b2RevoluteJoint_EnableMotor(temp,true);
    b2RevoluteJoint_SetMaxMotorTorque(temp,b2Body_GetMass(PhysicsUtils::getBodyId(this->registry,forearmRight))/15);
    b2RevoluteJoint_SetMotorSpeed(temp,0.0f);

    
}

Humanoid::~Humanoid()
{
}

void Humanoid::aim(b2Vec2 point)
{
}

void Humanoid::attack()
{
}

void Humanoid::defend()
{
}

void Humanoid::update(int FPS)
{
    keepTorsoUpright(FPS);
    keepHeadUpright(FPS);
    keepBodyAboveTheGround(this->legHeight);
}

float Humanoid::getHeightAboveTheGround()
{
    float ret = -1.0f;
    b2Vec2 posHipLeft = b2Body_GetWorldPoint(PhysicsUtils::getBodyId(this->registry, femurLeft), {0, 0});
    b2Vec2 posHipRight = b2Body_GetWorldPoint(PhysicsUtils::getBodyId(this->registry, femurRight), {0, 0});

    struct MyRaycastContext
    {
        int humanoidGroupId;
        float minDistance = FLT_MAX;
        b2Vec2 origin = {0, 0};
        b2ShapeId closestShape = b2_nullShapeId;
    } contextHipLeft, contextHipRight;
    contextHipLeft.humanoidGroupId = this->groupId;
    contextHipLeft.origin = posHipLeft;
    contextHipRight.humanoidGroupId = this->groupId;
    contextHipRight.origin = posHipRight;

    auto fcn = [](b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void *context) -> float
    {
        MyRaycastContext *myRaycastContext = static_cast<MyRaycastContext *>(context);
        if (b2Shape_GetFilter(shapeId).groupIndex != myRaycastContext->humanoidGroupId)
        {
            float dist = b2Distance(myRaycastContext->origin, point);
            if (dist < myRaycastContext->minDistance)
            {
                myRaycastContext->minDistance = dist;
                myRaycastContext->closestShape = shapeId;
            }
            return fraction;
        }
        else
        {
            return -1;
        }
    };

    // TODO: don't use a sizeYMeters*3, come up with something more general
    b2World_CastRay(this->worldId, posHipLeft, {0, -sizeYMeters*3}, b2DefaultQueryFilter(), fcn, &contextHipLeft);
    b2World_CastRay(this->worldId, posHipRight, {0, -sizeYMeters*3}, b2DefaultQueryFilter(), fcn, &contextHipRight);

    float dLeft = -1, dRight = -1;
    float n = 0;
    float s = 0;

    if (!B2_ID_EQUALS(contextHipLeft.closestShape, b2_nullShapeId))
    {
        dLeft = contextHipLeft.minDistance;
        s += dLeft;
        n++;
    }
    if (!B2_ID_EQUALS(contextHipRight.closestShape, b2_nullShapeId))
    {
        dRight = contextHipRight.minDistance;
        s += dRight;
        n++;
    }
    if (n != 0)
    {
        ret = s / n;
    }

    return ret;
}

void Humanoid::keepTorsoUpright(int FPS)
{
    b2BodyId torsoId = PhysicsUtils::getBodyId(this->registry, torso);

    float I = b2Body_GetRotationalInertia(torsoId);
    float omega_n = 6.0f; // rad/s: responsiveness
    float kp = I * omega_n * omega_n;
    float kd = 2 * I * omega_n;
    float targetAngle = 0.0f;
    PhysicsUtils::applyTorguePD(torsoId, targetAngle, kp, kd);
}

void Humanoid::keepHeadUpright(int FPS)
{
    b2BodyId neckId = PhysicsUtils::getBodyId(this->registry, neck);

    float I = b2Body_GetRotationalInertia(neckId);
    float omega_n = 8.0f; // rad/s: responsiveness
    float kp = I * omega_n * omega_n;
    float kd = 2 * I * omega_n;
    float targetAngle = 0.0f;
    PhysicsUtils::applyTorguePD(neckId, targetAngle, kp, kd);

    b2BodyId headId = PhysicsUtils::getBodyId(this->registry, head);

    I = b2Body_GetRotationalInertia(headId);
    omega_n = 4.0f; // rad/s: responsiveness
    kp = I * omega_n * omega_n;
    kd = 2 * I * omega_n;
    targetAngle = 0.0f;
    PhysicsUtils::applyTorguePD(headId, targetAngle, kp, kd);
}

// TODO: rewrite so it works properly
void Humanoid::keepBodyAboveTheGround(float targetHeightMeters)
{
    
    float curDistance = getHeightAboveTheGround();

    // No valid ground detected — abort
    if (curDistance < 0.0f)
        return;

    // Get torso body
    b2BodyId torsoBody = PhysicsUtils::getBodyId(this->registry, torso);

    // PD controller gains (you can tune these later)
    float kp = 2.3f*this->weightKg;
    float kd = 0.7f*this->weightKg;

    // Error and derivative
    float error = targetHeightMeters - curDistance;
    float velocity = b2Body_GetLinearVelocity(torsoBody).y;

    // Only apply upward force
    float force = kp * error - kd * velocity;
    if (force < 0.0f)
        force = 0.0f;

    // Apply upward force at torso center
    b2Vec2 forceVec = {0.0f, force};
    b2Body_ApplyForceToCenter(torsoBody, forceVec, true);
}

