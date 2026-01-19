#include "jointFactory.hpp"
#include "registryObjectFactory.hpp"

JointFactory::JointFactory(entt::registry &registry,
                           const std::shared_ptr<World> world)
    : RegistryObjectFactory(registry), world(world) {}
