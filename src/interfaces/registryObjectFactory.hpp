#pragma once
#include <entt/entt.hpp>
class RegistryObjectFactory {
public:
  virtual ~RegistryObjectFactory() = 0;

protected:
  RegistryObjectFactory(entt::registry &registry);
  entt::registry &registry;

private:
  RegistryObjectFactory() = delete;
  RegistryObjectFactory(RegistryObjectFactory &other) = delete;
  RegistryObjectFactory(RegistryObjectFactory &&other) = delete;
  RegistryObjectFactory operator=(RegistryObjectFactory &other) = delete;
  RegistryObjectFactory operator=(RegistryObjectFactory &&other) = delete;
};