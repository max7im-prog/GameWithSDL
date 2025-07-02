#pragma once
#include "bodyPart.hpp"
#include <optional>

class LimbBodyPart : public BodyPart
{
public:
    LimbBodyPart(entt::registry &registry,
                 b2WorldId worldId,
                 b2Vec2 worldPoint1,
                 b2Vec2 worldPoint2,
                 std::vector<std::pair<float,float>> portionRadiusPairs,
                 std::optional<b2Filter> shapeFilter = std::nullopt);
    ~LimbBodyPart();

    std::vector<std::pair<entt::entity,b2BodyId>> getBodies();
    std::vector<std::pair<entt::entity,b2JointId>> getJoints();
};