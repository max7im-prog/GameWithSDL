#pragma once
#include <unordered_map>

#include <box2d/box2d.h>
#include <entt/entt.hpp>

class BodyPart{
public:
    BodyPart(entt::registry &registry, b2WorldId worldId);
    virtual ~BodyPart();

    const virtual std::vector<std::pair<entt::entity,b2BodyId>> &getBodies();
    const virtual std::vector<std::pair<entt::entity,b2JointId>> &getJoints();
    
protected:
    std::vector<std::pair<entt::entity,b2BodyId>> bodies;
    std::vector<std::pair<entt::entity,b2JointId>> joints;
    entt::registry &registry;
    b2WorldId worldId;
    bool addJoint(std::pair<entt::entity, b2JointId> jointPair);
    bool addBody(std::pair<entt::entity, b2BodyId> bodyPair);
};