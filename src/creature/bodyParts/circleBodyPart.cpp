#include "circleBodyPart.hpp"
#include "physicsUtils.hpp"

CircleBodyPart::CircleBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint, float radiusM, std::optional<b2Filter> shapeFilter) : BodyPart(registry, worldId)
{
    auto ent = this->registry.create();
    auto pair = PhysicsUtils::createCirclePhysicsBody(this->registry, ent, this->worldId, worldPoint, {0, 0}, radiusM, shapeFilter);
    this->addBody(pair);
}

CircleBodyPart::~CircleBodyPart()
{
}

void CircleBodyPart::update(float dt)
{
}
