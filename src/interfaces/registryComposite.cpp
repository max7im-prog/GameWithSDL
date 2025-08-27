#include "registryComposite.hpp"
#include <memory>

void RegistryComposite::remove() {
  removeChildren();
  RegistryObject::remove();
}

RegistryComposite::~RegistryComposite() { removeChildren(); }

RegistryComposite::RegistryComposite(entt::registry &registry)
    : RegistryObject(registry) {}

void RegistryComposite::registerChild(std::shared_ptr<RegistryComposite> child) {
  if (children == nullptr) {
    children = std::make_unique<std::vector<std::shared_ptr<RegistryComposite>>>();
  }
  children->push_back(child);
}

void RegistryComposite::unregisterChild(std::shared_ptr<RegistryComposite> child) {
  if (children == nullptr) {
    return;
  }
  children->erase(std::remove(children->begin(), children->end(), child),
                  children->end());
}

void RegistryComposite::update(float dt) {
  if (children) {
    for (auto &ch : *children) {
      ch->update(dt);
    }
  }
}

void RegistryComposite::removeChildren() {
  if (children != nullptr) {
    for (auto &ch : *children) {
      ch->remove();
    }
  }
  children = nullptr;
}