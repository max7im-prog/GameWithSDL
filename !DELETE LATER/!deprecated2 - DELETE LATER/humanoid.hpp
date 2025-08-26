#pragma once

#include "baseCreature.hpp"

#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <cmath>

#include "bodyPart.hpp"
#include "capsuleBodyPart.hpp"
#include "circleBodyPart.hpp"
#include "polygonBodyPart.hpp"
#include "limbBodyPart.hpp"

#include "PIDScalarController.hpp"

class Humanoid : public BaseCreature
{
public:
    Humanoid(entt::registry &registry,
             entt::entity self,
             b2WorldId worldId,
             b2Vec2 position,
             float sizeXMeters = 2.0f,
             float sizeYMeters = 2.0f);
    ~Humanoid();

    void aim(b2Vec2 direction, bool aim) override;
    void attack() override;
    void defend() override;
    void update(float dt) override;
    void move(b2Vec2 direction, float speedMperSec, float accelerationMpS2) override;

protected:
    // Get a height of a torso above the ground in meters, returns -1 if could not find the ground or if too high
    float getHeightAboveTheGround();
    void keepTorsoAboveTheGround(float dt);
    void keepTorsoUpgright(float dt);
    void updateFeetPosition(float dt);

protected:
    float sizeXMeters;
    float sizeYMeters;
    float legHeight;
    float torsoHeight;

    float desiredSpeed;
    float currentAcceleration;
    b2Vec2 accelerationDir;

    std::shared_ptr<PolygonBodyPart> torso;
    std::shared_ptr<CapsuleBodyPart> neck;
    std::shared_ptr<PolygonBodyPart> head;

    std::shared_ptr<LimbBodyPart> leftArm;
    std::shared_ptr<LimbBodyPart> rightArm;

    std::shared_ptr<LimbBodyPart> leftLeg;
    std::shared_ptr<LimbBodyPart> rightLeg;

    PIDScalarController heightController;
    PIDScalarController torsoRotationController;

    struct LegMovementContext{
        enum Leg{
            left,right,
        }leg;
        int nextLeg = Leg::left;
        float stepSizeMtrs = 0;
        b2Vec2 lastStep = {0,0};
        b2Vec2 nextStep = {0,0};
    }legContext;
};