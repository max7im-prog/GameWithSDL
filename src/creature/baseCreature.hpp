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

    virtual void move(b2Vec2 direction, float speedMperSec, float accelerationMpS2) = 0;
    virtual void jump();
    virtual void aim(b2Vec2 direction) = 0;
    virtual void attack() = 0;
    virtual void defend() = 0;
    virtual void update(int FPS) = 0;

protected:
    b2WorldId worldId;
    b2Vec2 position;
    const int groupId;
    float weightKg;

    entt::entity self;
    entt::registry &registry;
    std::vector<std::pair<entt::entity,b2JointId>> joints;
    std::vector<std::shared_ptr<BodyPart>> bodyParts;

    // Creates an entity in registry and registres it in bodies. Does NOT attach a PhysicsBody to an entity
    // entt::entity createBodyEntity();

    // Creates a new revolute joint in a registry and 
    std::pair<entt::entity,b2JointId> connectRevolute(b2BodyId first, b2BodyId second, b2Vec2 worldPoint);

    std::pair<entt::entity,b2JointId> connectPrismatic(b2BodyId first, b2BodyId second, b2Vec2 worldPointFirst, b2Vec2 worldPointSecond);

    // Sets the weightKg value to be the sum of all weights of bodies in this->bodies
    void updateWeight();

    // Adds a body part's bodies and joint to the bodies and joints of a creature
    // void registerBodyPart(CapsuleBodyPart part);
    // void registerBodyPart(CircleBodyPart part);
    // void registerBodyPart(PolygonBodyPart part);
    // void registerBodyPart(LimbBodyPart part);

private:
    // std::vector<std::pair<entt::entity,b2BodyId>> bodies;
    
};