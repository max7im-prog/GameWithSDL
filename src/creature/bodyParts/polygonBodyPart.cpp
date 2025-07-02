#include "polygonBodyPart.hpp"

#include "physicsUtils.hpp"

// entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint, const std::vector<b2Vec2> vertices, std::optional<b2Filter> shapeFilter

PolygonBodyPart::PolygonBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint, const std::vector<b2Vec2> vertices, std::optional<b2Filter> shapeFilter):BodyPart(registry,worldId)
{
    auto ent = this->registry.create();
    auto pair = PhysicsUtils::createPolygonPhysicsBody(this->registry,ent,this->worldId,worldPoint,vertices,shapeFilter);
    this->addBodyToMap(pair);
}

PolygonBodyPart::~PolygonBodyPart()
{
}

std::pair<entt::entity, b2BodyId> PolygonBodyPart::getBody()
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