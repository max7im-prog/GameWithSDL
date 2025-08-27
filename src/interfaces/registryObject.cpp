#include "registryObject.hpp"

entt::entity RegistryObject::getEntity() { return entity; }

void RegistryObject::setEntity(entt::entity e) {
  if (isValid()) {
    registry.destroy(entity);
  }
  entity = e;
}

bool RegistryObject::isValid() const { return registry.valid(entity); }

void RegistryObject::remove() {
  if (isValid()) {
    registry.destroy(entity);
  }
  entity = entt::null;
}

RegistryObject::~RegistryObject() { remove(); }

RegistryObject::RegistryObject(entt::registry &registry)
    : registry(registry), entity(entt::null) {}