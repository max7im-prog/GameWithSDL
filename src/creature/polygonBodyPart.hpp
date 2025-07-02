#pragma once
#include "bodyPart.hpp"
#include <optional>

class PolygonBodyPart : public BodyPart
{
public:
    PolygonBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint,b2Vec2 center1,
                                                                     b2Vec2 center2,
                                                                     float radius, std::optional<b2Filter> shapeFilter = std::nullopt);
    ~PolygonBodyPart();
};