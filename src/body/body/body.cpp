#include "body.hpp"
#include "registryObject.hpp"

Body::~Body() {
}

Body::Body(entt::registry &registry, const std::shared_ptr<World> world)
    : RegistryObject(registry), world(world) {}