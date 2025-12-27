#include "registryObject.hpp"

entt::entity RegistryObject::getEntity() { return _entity; }

void RegistryObject::setEntity(entt::entity e) {
  if (isValid()) {
    _registry.destroy(_entity);
  }
  _entity = e;
}

bool RegistryObject::isValid() const { return _registry.valid(_entity); }

void RegistryObject::remove() {
  if (!_registry.valid(_entity)) {
    _entity = entt::null;
    return;
  }
  _registry.destroy(_entity);
  _entity = entt::null;
}

RegistryObject::~RegistryObject() { remove(); }

RegistryObject::RegistryObject(entt::registry &registry)
    : _registry(registry), _entity(entt::null) {}
