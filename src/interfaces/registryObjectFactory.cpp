#include "registryObjectFactory.hpp"

RegistryObjectFactory::RegistryObjectFactory(entt::registry &registry)
    : registry(registry) {}

RegistryObjectFactory::~RegistryObjectFactory() {}

template <typename T>
std::shared_ptr<T> RegistryObjectFactory::create(
    const typename FacturyConfigMapping<T>::Config &config) {
  return nullptr;
}

template <typename T>
void RegistryObjectFactory::registerObject(const std::shared_ptr<T> object) {}