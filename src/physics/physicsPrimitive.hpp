#pragma once

#include "objectConfig.hpp"
#include "registryComposite.hpp"
class PhysicsPrimitive : public RegistryComposite {
public:
  class Config : public ObjectConfig {};
  virtual ~PhysicsPrimitive() = 0;

protected:
  PhysicsPrimitive(entt::registry &registry);
};
