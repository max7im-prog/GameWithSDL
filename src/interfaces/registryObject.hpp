#pragma once

#include <entt/entt.hpp>

/**
 * @brief Interface to store objects that have to be registered in
 * entt::registry.
 *
 */
class RegistryObject {
public:
  virtual ~RegistryObject();
  entt::entity getEntity();

  /**
   * @brief Checks if an object is a valid entry in a registry
   *
   */
  bool isValid() const;

  /**
   * @brief removes object's entry from registry.
   *
   */
  virtual void remove();

protected:
  RegistryObject(entt::registry &registry);
  void setEntity(entt::entity e);
  entt::registry &registry;
  entt::entity entity;

private:
  RegistryObject() = delete;
  RegistryObject(RegistryObject &other) = delete;
  RegistryObject(RegistryObject &&other) = delete;
  RegistryObject &operator=(RegistryObject &other) = delete;
  RegistryObject &operator=(RegistryObject &&other) = delete;
};