#include "capsuleBodyPart.hpp"
#include "physicsUtils.hpp"

CapsuleBodyPart::CapsuleBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint, b2Vec2 center1, b2Vec2 center2, float radius, std::optional<b2Filter> shapeFilter) : BodyPart(registry, worldId)
{
    auto ent = this->registry.create();
    auto pair = PhysicsUtils::createCapsulePhysicsBody(this->registry, ent, this->worldId, worldPoint, center1, center2, radius, shapeFilter);
    this->addBodyToMap(pair);
}

CapsuleBodyPart::~CapsuleBodyPart()
{
}

std::pair<entt::entity, b2BodyId> CapsuleBodyPart::getBody()
{
    if (this->bodies.size() != 0)
    {
        for (auto pair : this->bodies)
            return pair;
    }
    else
    {
        return {};
    }
}