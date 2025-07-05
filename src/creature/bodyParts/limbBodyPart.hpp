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
    
    // Returns a position of a base of a limb
    b2Vec2 getBase();

    // Returns a position of an end of a limb
    b2Vec2 getEnd();

    // Aligns the outmost part of a limb with a direction to a worldPoint and tries to move it there
    void pointAt(b2Vec2 worldPoint);

    // const std::vector<std::pair<entt::entity,b2BodyId>> &getBodies();
    // const std::vector<std::pair<entt::entity,b2JointId>> &getJoints();
};