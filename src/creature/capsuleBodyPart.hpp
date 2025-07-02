#pragma once
#include "bodyPart.hpp"
#include <optional>

class CapsuleBodyPart : public BodyPart
{
public:
    CapsuleBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint, const std::vector<b2Vec2> vertices, std::optional<b2Filter> shapeFilter = std::nullopt);
    ~CapsuleBodyPart();
};