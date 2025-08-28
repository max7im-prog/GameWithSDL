#pragma once

#include "registryObject.hpp"
class RegistryContainer : public RegistryObject {
public:
  virtual ~RegistryContainer();

  /**
   * @brief Registers an object in a container.
   */
  void addObject(const std::shared_ptr<RegistryObject> &object);
  void addObject(const std::weak_ptr<RegistryObject> &object);

  /**
   * @brief Removes object from a container by calling remove() on an object.
   */
  void removeObject(std::shared_ptr<RegistryObject> object);
  void removeObject(std::weak_ptr<RegistryObject> object);

  /**
   * @brief Removes all the objects from the RegistryContainer
   *
   */
  void clear();

protected:
  RegistryContainer(entt::registry &registry);

private:
  std::vector<std::weak_ptr<RegistryObject>> objects;
};