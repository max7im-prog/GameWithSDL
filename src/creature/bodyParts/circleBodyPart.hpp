#pragma once
#include "bodyPart.hpp"
#include <optional>

class CircleBodyPart:public BodyPart{
public:
    CircleBodyPart(entt::registry &registry,b2WorldId worldId, b2Vec2 worldPoint,float radiusM,std::optional<b2Filter> shapeFilter = std::nullopt);
    ~CircleBodyPart();
    
    std::vector<std::pair<entt::entity,b2BodyId>> getBodies();
    std::vector<std::pair<entt::entity,b2JointId>> getJoints();
};