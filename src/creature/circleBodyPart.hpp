#pragma once
#include "bodyPart.hpp"

class CircleBodyPart:public BodyPart{
public:
    CircleBodyPart(entt::registry &registry,b2WorldId worldId, b2Vec2 worldPoint,float radiusM,std::optional<b2Filter> shapeFilter = std::nullopt);
    ~CircleBodyPart();
};