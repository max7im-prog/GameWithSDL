#include "humanoid.hpp"
#include "physicsUtils.hpp"
#include "baseCreature.hpp"
#include "physicsComponents.hpp"

constexpr float DEFAULT_TORSO_HEIGHT_RESPONSIVENESS = 1.1f;
constexpr float DEFAULT_TORSO_ROTATION_RESPONSIVENESS = 3.0f;

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
    this->legHeight = (measureY * 6.0f + 0.5f * measure);

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
    {
        neck = std::make_shared<CapsuleBodyPart>(
            this->registry,
            worldId,
            posNeckBase,
            b2Vec2{0, 0},
            b2Sub(posHeadBase, position),
            0.5f * measure,
            filter);
        this->bodyParts.push_back(neck);
    }
    {
        std::vector<b2Vec2> headShape = {{-1.0f * measureX, 0}, {-1.0f * measureX, 2.0f * measureY}, {1.0f * measureX, 2.0f * measureY}, {1.0f * measureX, 0}};
        head = std::make_shared<PolygonBodyPart>(
            this->registry, worldId, posHeadBase, headShape, filter);
        this->bodyParts.push_back(head);
    }
    {
        std::vector<b2Vec2> torsoShape = {{0, 0}, {-2.0f * measureX, 5.0f * measureY}, {2.0f * measureX, 5.0f * measureY}};
        torso = std::make_shared<PolygonBodyPart>(
            this->registry, worldId, posTorsoBase, torsoShape, filter);
        this->bodyParts.push_back(torso);
        this->torsoHeight = 5.0f * measureY;
    }

    // Limbs
    {
        std::vector<std::pair<float, float>> portionRadiusPairs = {{0.5, 0.5F * measure}, {0.5, 0.5F * measure}};
        leftLeg = std::make_shared<LimbBodyPart>(
            this->registry, worldId, posLeftHip, posLeftFoot, portionRadiusPairs, filter);
        this->bodyParts.push_back(leftLeg);
    }
    {
        std::vector<std::pair<float, float>> portionRadiusPairs = {{0.5, 0.5F * measure}, {0.5, 0.5F * measure}};
        rightLeg = std::make_shared<LimbBodyPart>(
            this->registry, worldId, posRightHip, posRightFoot, portionRadiusPairs, filter);
        this->bodyParts.push_back(rightLeg);
    }
    {
        std::vector<std::pair<float, float>> portionRadiusPairs = {{0.5, 0.5F * measure}, {0.5, 0.5F * measure}};
        leftArm = std::make_shared<LimbBodyPart>(
            this->registry, worldId, posLeftShoulder, posLeftPalm, portionRadiusPairs, filter);
        this->bodyParts.push_back(leftArm);
    }
    {
        std::vector<std::pair<float, float>> portionRadiusPairs = {{0.5, 0.5F * measure}, {0.5, 0.5F * measure}};
        rightArm = std::make_shared<LimbBodyPart>(
            this->registry, worldId, posRightShoulder, posRightPalm, portionRadiusPairs, filter);
        this->bodyParts.push_back(rightArm);
    }
    this->updateWeight();

    // Connect limbs
    {
        auto bodies1 = neck->getBodies();
        auto bodies2 = head->getBodies();
        if (bodies1.size() > 0 && bodies2.size() > 0)
        {
            auto pair = connectRevolute(bodies1[0].second, bodies2[0].second, posHeadBase);
            this->joints.push_back(pair);
            b2RevoluteJoint_SetLimits(pair.second, -0.1f, 0.1f);
            b2RevoluteJoint_EnableLimit(pair.second, true);
        }
    }
    {
        auto bodies1 = neck->getBodies();
        auto bodies2 = torso->getBodies();
        if (bodies1.size() > 0 && bodies2.size() > 0)
        {
            auto pair = connectRevolute(bodies1[0].second, bodies2[0].second, posNeckBase);
            this->joints.push_back(pair);
            b2RevoluteJoint_SetLimits(pair.second, -0.1f, 0.1f);
            b2RevoluteJoint_EnableLimit(pair.second, true);
        }
    }
    {
        auto bodies1 = leftArm->getBodies();
        auto bodies2 = torso->getBodies();
        if (bodies1.size() > 0 && bodies2.size() > 0)
        {
            auto pair = leftArm->connect(bodies2[0].second, posLeftShoulder);
            if (pair.first != entt::null)
            {
                this->joints.push_back(pair);
                b2RevoluteJoint_SetMaxMotorTorque(pair.second, b2Body_GetMass(bodies1[0].second) / 11);
                b2RevoluteJoint_SetMotorSpeed(pair.second, 0.0f);
            }
        }
    }
    {
        auto bodies1 = rightArm->getBodies();
        auto bodies2 = torso->getBodies();
        if (bodies1.size() > 0 && bodies2.size() > 0)
        {
            auto pair = rightArm->connect(bodies2[0].second, posRightShoulder);
            if (pair.first != entt::null)
            {
                this->joints.push_back(pair);
                b2RevoluteJoint_SetMaxMotorTorque(pair.second, b2Body_GetMass(bodies1[0].second) / 11);
                b2RevoluteJoint_SetMotorSpeed(pair.second, 0.0f);
            }
        }
    }
    {
        auto bodies1 = leftLeg->getBodies();
        auto bodies2 = torso->getBodies();
        if (bodies1.size() > 0 && bodies2.size() > 0)
        {
            auto pair = leftLeg->connect(bodies2[0].second, posLeftHip);
            if (pair.first != entt::null)
            {
                this->joints.push_back(pair);
                b2RevoluteJoint_SetMaxMotorTorque(pair.second, b2Body_GetMass(bodies1[0].second) / 11);
                b2RevoluteJoint_SetMotorSpeed(pair.second, 0.0f);
            }
        }
    }
    {
        auto bodies1 = rightLeg->getBodies();
        auto bodies2 = torso->getBodies();
        if (bodies1.size() > 0 && bodies2.size() > 0)
        {
            auto pair = rightLeg->connect(bodies2[0].second, posRightHip);
            if (pair.first != entt::null)
            {
                this->joints.push_back(pair);
                b2RevoluteJoint_SetMaxMotorTorque(pair.second, b2Body_GetMass(bodies1[0].second) / 11);
                b2RevoluteJoint_SetMotorSpeed(pair.second, 0.0f);
            }
        }
    }

    // PID controllers
    float gravity = b2Length(b2World_GetGravity(this->worldId));
    {
        float mass = this->weightKg;
        float omega_n = DEFAULT_TORSO_HEIGHT_RESPONSIVENESS;
        float kp = mass * gravity * omega_n * omega_n;
        float kd = 2.0f * mass * omega_n;
        float ki = kp*0.1;
        float maxForce = mass*gravity*4;
        this->heightController = PIDScalarController(kp, ki, kd,maxForce);
    }
    {
        float mass = this->weightKg;
        float omega_n = DEFAULT_TORSO_ROTATION_RESPONSIVENESS;
        float kp = mass * gravity * omega_n * omega_n;
        float kd = 2.0f * mass * omega_n;
        float ki = 0;
        this->torsoRotationController = PIDScalarController(kp, ki, kd);
    }
}

Humanoid::~Humanoid()
{
}

void Humanoid::aim(b2Vec2 direction, bool aim)
{
    this->leftArm->trackPoint(direction, aim);
    this->rightArm->trackPoint(direction, aim);
}

void Humanoid::attack()
{
}

void Humanoid::defend()
{
}

void Humanoid::update(float dt)
{
    // keepTorsoUpright(FPS);
    // keepHeadUpright(FPS);
    // keepBodyAboveTheGround(this->legHeight);
    // applyForceToTorso();
    // this->leftArm->updateTracking({5.0f, 5.0f});
    // this->leftLeg->updateTracking({5.0f, 5.0f});
    // this->rightArm->updateTracking({5.0f, 5.0f});
    // this->rightLeg->updateTracking({5.0f, 5.0f});
    this->keepTorsoAboveTheGround(dt);
    this->keepTorsoUpgright(dt);
    for (auto bp : this->bodyParts)
    {
        bp->update(dt);
    }
}

float Humanoid::getHeightAboveTheGround()
{

    float ret = -1.0f;

    b2Vec2 posLeftHip = this->leftLeg->getBase();
    b2Vec2 posRightHip = this->rightLeg->getBase();

    struct MyRaycastContext
    {
        int humanoidGroupId;
        float minDistance = FLT_MAX;
        b2Vec2 origin = {0, 0};
        b2ShapeId closestShape = b2_nullShapeId;
    } contextHipLeft, contextHipRight;
    contextHipLeft.humanoidGroupId = this->groupId;
    contextHipLeft.origin = posLeftHip;
    contextHipRight.humanoidGroupId = this->groupId;
    contextHipRight.origin = posRightHip;

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
    b2World_CastRay(this->worldId, posLeftHip, {0, -sizeYMeters * 3}, b2DefaultQueryFilter(), fcn, &contextHipLeft);
    b2World_CastRay(this->worldId, posRightHip, {0, -sizeYMeters * 3}, b2DefaultQueryFilter(), fcn, &contextHipRight);

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

void Humanoid::keepTorsoAboveTheGround(float dt)
{
    float curHeight  = getHeightAboveTheGround();
    if(curHeight <0){
        return;
    }
    float error = this->legHeight*0.8-curHeight;
    
    if(this->legHeight-curHeight <0 &&std::abs(this->legHeight-curHeight)/this->legHeight >0.8){
        this->heightController.reset();
        return;
    }
    float force = this->heightController.update(error,dt);
    b2BodyId torsoId = this->torso->getBodies()[0].second;
    b2Body_ApplyForce(torsoId,b2MulSV(force,{0,1}),b2Body_GetWorldPoint(torsoId,b2MulSV(torsoHeight,{0,-1})),true);
}

void Humanoid::keepTorsoUpgright(float dt)
{
    b2BodyId torsoId = this->torso->getBodies()[0].second;
    float error =  -b2Rot_GetAngle(b2Body_GetRotation(torsoId));
    float torque = this->torsoRotationController.update(error,dt);
    b2Body_ApplyTorque(torsoId,torque,true);
}

void Humanoid::move(b2Vec2 direction, float speedMperSec, float accelerationMpS2)
{
    accelerationDir = direction;
    desiredSpeed = speedMperSec;
    currentAcceleration = accelerationMpS2;
}
