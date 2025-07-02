#pragma once
#include <unordered_map>

#include <box2d/box2d.h>
#include <entt/entt.hpp>

class BodyPart{
public:
    BodyPart(entt::registry &registry, b2WorldId worldId);
    virtual ~BodyPart();
    virtual std::vector<std::pair<entt::entity,b2BodyId>> getBodies() = 0;
    virtual std::vector<std::pair<entt::entity,b2JointId>> getJoints() = 0;
protected:
    std::unordered_map<entt::entity,b2BodyId> bodies;
    std::unordered_map<entt::entity,b2JointId> joints;
    entt::registry &registry;
    b2WorldId worldId;
    bool addJointToMap(std::pair<entt::entity, b2JointId> jointPair);
    bool addBodyToMap(std::pair<entt::entity, b2BodyId> bodyPair);

};