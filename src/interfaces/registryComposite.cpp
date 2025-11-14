#include "registryComposite.hpp"
#include <memory>

void RegistryComposite::remove() {
  removeChildren();
  RegistryObject::remove();
}

RegistryComposite::~RegistryComposite() { removeChildren(); }

RegistryComposite::RegistryComposite(entt::registry &registry)
    : RegistryObject(registry) {}

void RegistryComposite::registerChild(std::weak_ptr<RegistryComposite> child) {
  if (children == nullptr) {
    children =
        std::make_unique<std::vector<std::weak_ptr<RegistryComposite>>>();
  }
  children->push_back(child);
}
std::vector<std::weak_ptr<RegistryComposite>>
RegistryComposite::getChildren() {
  if(!children){
   return {};
  }
  return *children;
}

void RegistryComposite::unregisterChild(
    std::weak_ptr<RegistryComposite> child) {
  if (children == nullptr) {
    return;
  }

  auto childLocked = child.lock();
  std::erase_if(*children,
                [&childLocked](std::weak_ptr<RegistryComposite> cur) -> bool {
                  std::shared_ptr<RegistryComposite> curLocked;
                  curLocked = cur.lock();
                  if (!curLocked) {
                    return true;
                  }
                  if (!childLocked) {
                    return false;
                  }
                  return childLocked == curLocked;
                });
}

void RegistryComposite::update(float dt) {
  if (children) {
    for (auto &ch : *children) {
      auto chLocked = ch.lock();
      if (chLocked) {
        chLocked->update(dt);
      }
    }
  }
}

void RegistryComposite::removeChildren() {
  if (children != nullptr) {
    for (auto &ch : *children) {
      auto chLocked = ch.lock();
      if (chLocked) {
        chLocked->remove();
      }
    }
  }
  children = nullptr;
}