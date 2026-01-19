#pragma once

#include "objectConfig.hpp"
#include "registryComposite.hpp"
class Structure : public RegistryComposite {
public:
  class Config : public ObjectConfig {};
  virtual ~Structure() = 0;

protected:
  Structure(entt::registry &registry);
};
