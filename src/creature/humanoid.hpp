#pragma once

#include "baseCreature.hpp"

#include <entt/entt.hpp>
#include <box2d/box2d.h>

#include "bodyPart.hpp"
#include "capsuleBodyPart.hpp"
#include "circleBodyPart.hpp"
#include "polygonBodyPart.hpp"
#include "limbBodyPart.hpp"

class Humanoid: public BaseCreature{
public:
    Humanoid(entt::registry &registry, 
        entt::entity self,
        b2WorldId worldId,
        b2Vec2 position,
        float sizeXMeters = 2.0f,
        float sizeYMeters = 2.0f);
    ~Humanoid();

    void aim(b2Vec2 point) override;
    void attack() override;
    void defend() override;
    void update(int FPS) override;
    void move(b2Vec2 direction, float speedMperSec, float accelerationMpS2) override;
protected:
    // Get a height of a torso above the ground in meters, returns -1 if could not find the ground or if too high
    float getHeightAboveTheGround();

    void keepTorsoUpright(int FPS);
    void keepHeadUpright(int FPS);
    void keepBodyAboveTheGround(float heightMeters);

    void applyForceToTorso();
    


protected:
    float sizeXMeters;
    float sizeYMeters;

    // Used by keepBodyAboveTheGround() function
    float legHeight;

    float desiredSpeed;
    float currentAcceleration;
    b2Vec2 accelerationDir;

    std::shared_ptr<PolygonBodyPart> torso;
    std::shared_ptr<CapsuleBodyPart>  neck;
    std::shared_ptr<PolygonBodyPart>  head;

    std::shared_ptr<LimbBodyPart> leftArm;
    std::shared_ptr<LimbBodyPart> rightArm;

    std::shared_ptr<LimbBodyPart> leftLeg;
    std::shared_ptr<LimbBodyPart> rightLeg;
};