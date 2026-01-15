#include "worldFactory.hpp"
#include "registryObjectFactory.hpp"

WorldFactory::WorldFactory(entt::registry &registry)
    : RegistryObjectFactory(registry) {}
