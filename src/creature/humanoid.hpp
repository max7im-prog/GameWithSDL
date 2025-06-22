#pragma once

#include "baseCreature.hpp"

#include <entt/entt.hpp>
#include <box2d/box2d.h>

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
protected:
    void keepTorsoUpright(int FPS);
    void keepHeadUpright(int FPS);
    


protected:
    entt::entity torso;
    entt::entity neck;
    entt::entity head;

    entt::entity shoulderLeft;
    entt::entity upperArmLeft;
    entt::entity elbowLeft;
    entt::entity forearmLeft;
    entt::entity palmLeft;
    entt::entity hipLeft;
    entt::entity femurLeft;
    entt::entity kneeLeft;
    entt::entity calfLeft;
    entt::entity footLeft;

    entt::entity shoulderRight;
    entt::entity upperArmRight;
    entt::entity elbowRight;
    entt::entity forearmRight;
    entt::entity palmRight;
    entt::entity hipRight;
    entt::entity femurRight;
    entt::entity kneeRight;
    entt::entity calfRight;
    entt::entity footRight;
};