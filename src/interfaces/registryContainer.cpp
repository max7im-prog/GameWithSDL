#include "registryContainer.hpp"
#include "registryObject.hpp"

RegistryContainer::RegistryContainer(entt::registry &registry)
    : RegistryObject(registry) {}

RegistryContainer::~RegistryContainer() { clear(); }

void RegistryContainer::addObject(std::shared_ptr<RegistryObject> object) {
  if (!object)
    return;
  auto raw = object.get();

  // Only add if not already present
  auto it = std::find_if(objects.begin(), objects.end(),
                         [&](const std::weak_ptr<RegistryObject> &wptr) {
                           auto sp = wptr.lock();
                           return sp && sp.get() == raw;
                         });
  if (it == objects.end()) {
    objects.push_back(object);
  }
}

void RegistryContainer::addObject(std::weak_ptr<RegistryObject> object) {
  addObject(object.lock());
}

void RegistryContainer::removeObject(std::shared_ptr<RegistryObject> object) {
  if (!object)
    return;

  objects.erase(std::remove_if(objects.begin(), objects.end(),
                               [&](const std::weak_ptr<RegistryObject> &wptr) {
                                 auto sp = wptr.lock();
                                 if (!sp || sp == object) {
                                   if (sp)
                                     sp->remove(); // propagate removal
                                   return true;    // erase from vector
                                 }
                                 return false;
                               }),
                objects.end());
}

void RegistryContainer::removeObject(std::weak_ptr<RegistryObject> object) {
  removeObject(object.lock());
}

void RegistryContainer::clear() {
  for (auto &obj : objects) {
    if (auto ptr = obj.lock()) {
      ptr->remove();
    }
  }
  objects = {};
}