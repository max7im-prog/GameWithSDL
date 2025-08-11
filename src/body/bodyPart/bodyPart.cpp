#include "bodyPart.hpp"
#include "registryObject.hpp"

BodyPart::~BodyPart() {
}

BodyPart::BodyPart(entt::registry &registry, const std::shared_ptr<World> world)
    : RegistryObject(registry), world(world) {}