#pragma once
#include <entt/entt.hpp>
#include <box2d/box2d.h>

class BaseCreature
{
public:
    BaseCreature(entt::registry &registry, entt::entity self, b2WorldId worldId, b2Vec2 position);
    virtual ~BaseCreature();

    virtual void move(b2Vec2 direction);
    virtual void jump();
    virtual void aim(b2Vec2 direction) = 0;
    virtual void attack() = 0;
    virtual void defend() = 0;
    virtual void update() = 0;

protected:
    b2WorldId worldId;
    b2Vec2 position;

    entt::entity self;
    entt::registry &registry;
    std::vector<entt::entity> bodies;
    std::vector<entt::entity> joints;

    bool connectRevolute(entt::entity first, entt::entity second, b2Vec2 worldPoint);
};