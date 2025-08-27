#include "registryObject.hpp"
#include <memory>

void RegistryObject::remove() {
  removeChildren();
  RegistryBase::remove();
}

RegistryObject::~RegistryObject() { removeChildren(); }

RegistryObject::RegistryObject(entt::registry &registry)
    : RegistryBase(registry) {}

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

void RegistryObject::removeChildren() {
  if (children != nullptr) {
    for (auto &ch : *children) {
      ch->remove();
    }
  }
  children = nullptr;
}