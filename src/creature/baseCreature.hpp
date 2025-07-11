#pragma once
#include <entt/entt.hpp>
#include <box2d/box2d.h>

#include "bodyPart.hpp"
#include "capsuleBodyPart.hpp"
#include "circleBodyPart.hpp"
#include "polygonBodyPart.hpp"
#include "limbBodyPart.hpp"

class BaseCreature
{
public:
    BaseCreature(entt::registry &registry, entt::entity self, b2WorldId worldId, b2Vec2 position, int groupId);
    virtual ~BaseCreature();
    BaseCreature(BaseCreature& other) = delete;
    BaseCreature(BaseCreature&& other) = delete;
    BaseCreature& operator=(BaseCreature& other) = delete;
    BaseCreature&& operator=(BaseCreature&& other) = delete;

    virtual void move(b2Vec2 direction, float speedMperSec, float accelerationMpS2) = 0;
    virtual void jump();
    virtual void aim(b2Vec2 direction, bool aim) = 0;
    virtual void attack() = 0;
    virtual void defend() = 0;
    virtual void update(float dt) = 0;

protected:
    b2WorldId worldId;
    b2Vec2 position;
    const int groupId;
    float weightKg;

    entt::entity self;
    entt::registry &registry;
    std::vector<std::pair<entt::entity, b2JointId>> joints;
    std::vector<std::shared_ptr<BodyPart>> bodyParts;

    std::pair<entt::entity, b2JointId> connectRevolute(b2BodyId first, b2BodyId second, b2Vec2 worldPoint);

    std::pair<entt::entity, b2JointId> connectPrismatic(b2BodyId first, b2BodyId second, b2Vec2 worldPointFirst, b2Vec2 worldPointSecond);

    // Sets the weightKg value to be the sum of all weights of bodies in this->bodies
    void updateWeight();

private:
};