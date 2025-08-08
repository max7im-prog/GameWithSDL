#include "registryObject.hpp"

entt::entity RegistryObject::getEntity() { return entity; }

bool RegistryObject::isValid() const { return registry.valid(entity); }

void RegistryObject::remove() {
  if (isValid()) {
    registry.destroy(entity);
  }
  entity = entt::null;
}

RegistryObject::~RegistryObject() { remove(); }

RegistryObject::RegistryObject(entt::registry &registry) : registry(registry), entity(entt::null) {}

void RegistryObject::setEntity(entt::entity e) { entity = e; }