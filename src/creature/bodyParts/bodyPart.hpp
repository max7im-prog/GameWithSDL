#pragma once

#include <box2d/box2d.h>
#include <entt/entt.hpp>

class BodyPart{
public:
    BodyPart(entt::registry &registry, b2WorldId worldId);
    BodyPart(BodyPart& other) = delete;
    BodyPart(BodyPart&& other) = delete;
    BodyPart& operator=(BodyPart& other) = delete;
    BodyPart&& operator=(BodyPart&& other) = delete;
    virtual ~BodyPart();

    const virtual std::vector<std::pair<entt::entity,b2BodyId>> &getBodies();
    const virtual std::vector<std::pair<entt::entity,b2JointId>> &getJoints();

    virtual void update(float dt) = 0;

    float getMassKg();
    
protected:
    std::vector<std::pair<entt::entity,b2BodyId>> bodies;
    std::vector<std::pair<entt::entity,b2JointId>> joints;
    entt::registry &registry;
    b2WorldId worldId;
    bool addJoint(std::pair<entt::entity, b2JointId> jointPair);
    bool addBody(std::pair<entt::entity, b2BodyId> bodyPair);

    float weightKg = 0;
};