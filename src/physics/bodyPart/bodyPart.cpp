#include "bodyPart.hpp"

BodyPart::~BodyPart() {
  if (registry.valid(entity)) {
    registry.destroy(entity);
  }
}

entt::entity BodyPart::getEntity() { return entity; }

BodyPart::BodyPart(entt::registry &registry, const std::shared_ptr<World> world)
    : world(world), registry(registry) {}