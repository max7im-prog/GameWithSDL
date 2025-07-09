#pragma once
#include "bodyPart.hpp"
#include <optional>

class CapsuleBodyPart : public BodyPart
{
public:
    CapsuleBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint, b2Vec2 center1,b2Vec2 center2,float radius, std::optional<b2Filter> shapeFilter);
    ~CapsuleBodyPart();
    void update(float dt) override;
    // const std::vector<std::pair<entt::entity,b2BodyId>> &getBodies();
    // const std::vector<std::pair<entt::entity,b2JointId>> &getJoints();
};