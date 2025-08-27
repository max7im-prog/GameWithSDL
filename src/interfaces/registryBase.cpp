#include "registryBase.hpp"

entt::entity RegistryBase::getEntity() { return entity; }

void RegistryBase::setEntity(entt::entity e) {
  if (isValid()) {
    registry.destroy(entity);
  }
  entity = e;
}

bool RegistryBase::isValid() const { return registry.valid(entity); }
