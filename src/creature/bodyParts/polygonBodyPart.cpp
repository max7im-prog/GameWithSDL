#include "polygonBodyPart.hpp"

#include "physicsUtils.hpp"

PolygonBodyPart::PolygonBodyPart(entt::registry &registry, b2WorldId worldId,
                                 b2Vec2 worldPoint,
                                 const std::vector<b2Vec2> vertices,
                                 std::optional<b2Filter> shapeFilter)
    : BodyPart(registry, worldId) {
  auto ent = this->registry.create();
  auto pair = PhysicsUtils::createPolygonPhysicsBody(
      this->registry, ent, this->worldId, worldPoint, vertices, shapeFilter);
  this->addBody(pair);
}

PolygonBodyPart::~PolygonBodyPart() {}

void PolygonBodyPart::update(float dt) {}

std::shared_ptr<PolygonBodyPart>
PolygonBodyPart::create(entt::registry &registry, b2WorldId worldId,
                        b2Vec2 worldPoint, const std::vector<b2Vec2> vertices,
                        std::optional<b2Filter> shapeFilter) {
  return std::shared_ptr<PolygonBodyPart>(new PolygonBodyPart(
      registry, worldId, worldPoint, vertices, shapeFilter));
}
