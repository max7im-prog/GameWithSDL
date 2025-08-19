#include "registryObjectFactory.hpp"

RegistryObjectFactory::RegistryObjectFactory(entt::registry &registry)
    : registry(registry) {}

RegistryObjectFactory::~RegistryObjectFactory() {}