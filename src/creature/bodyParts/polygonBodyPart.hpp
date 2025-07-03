#pragma once
#include "bodyPart.hpp"
#include <optional>

class PolygonBodyPart : public BodyPart
{
public:
    PolygonBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint, const std::vector<b2Vec2> vertices, std::optional<b2Filter> shapeFilter = std::nullopt);
    ~PolygonBodyPart();
    // const std::vector<std::pair<entt::entity,b2BodyId>> &getBodies();
    // const std::vector<std::pair<entt::entity,b2JointId>> &getJoints();
};