#pragma once

#include <entt/entt.hpp>

class RegistryBase {
public:
  virtual ~RegistryBase() = 0;
  entt::entity getEntity();

  /**
   * @brief Checks if an object is a valid entry in a registry
   *
   */
  bool isValid() const;

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