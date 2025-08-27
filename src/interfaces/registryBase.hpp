#pragma once

#include <entt/entt.hpp>

/**
 * @brief Interface to store objects that have to be registered in
 * entt::registry.
 *
 */
class RegistryBase {
public:
  virtual ~RegistryBase();
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
  RegistryBase(entt::registry &registry);
  void setEntity(entt::entity e);
  entt::registry &registry;
  entt::entity entity;

private:
  RegistryBase() = delete;
  RegistryBase(RegistryBase &other) = delete;
  RegistryBase(RegistryBase &&other) = delete;
  RegistryBase &operator=(RegistryBase &other) = delete;
  RegistryBase &operator=(RegistryBase &&other) = delete;
};