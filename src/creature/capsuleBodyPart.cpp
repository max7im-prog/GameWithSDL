#include "capsuleBodyPart.hpp"

CapsuleBodyPart::CapsuleBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint, const std::vector<b2Vec2> vertices, std::optional<b2Filter> shapeFilter):BodyPart(registry,worldId)
{
}

CapsuleBodyPart::~CapsuleBodyPart()
{
}
