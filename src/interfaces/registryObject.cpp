#include "registryObject.hpp"
#include <memory>

entt::entity RegistryObject::getEntity() { return entity; }

bool RegistryObject::isValid() const { return registry.valid(entity); }

void RegistryObject::remove() {
  if (children != nullptr) {
    for (auto &ch : *children) {
      ch->remove();
    }
  }

  if (isValid()) {
    registry.destroy(entity);
  }
  entity = entt::null;
}

RegistryObject::~RegistryObject() { remove(); }

RegistryObject::RegistryObject(entt::registry &registry)
    : registry(registry), entity(entt::null) {}

void RegistryObject::setEntity(entt::entity e) {
  if (isValid()) {
    registry.destroy(entity);
  }
  entity = e;
}

void RegistryObject::registerChild(std::shared_ptr<RegistryObject> child) {
  if (children == nullptr) {
    children = std::make_unique<std::vector<std::shared_ptr<RegistryObject>>>();
  }
  children->push_back(child);
}

void RegistryObject::unregisterChild(std::shared_ptr<RegistryObject> child) {
  if (children == nullptr) {
    return;
  }
  children->erase(std::remove(children->begin(), children->end(), child),
                  children->end());
}

void RegistryObject::update(float dt) {
  if (children) {
    for (auto &ch : *children) {
      ch->update(dt);
    }
  }
}