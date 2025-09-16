#pragma once
#include <box2d/box2d.h>
#include "registryContainer.hpp"


struct WorldConfig {
  uint32_t substepCount = 4;
};

class World : public RegistryContainer {
public:
  b2WorldId getWorldId() const;
  virtual ~World();
  void step(float dt);

protected:
  World(entt::registry &registry);
  b2WorldId worldId;
  uint32_t substepCount = 1;

private:
  friend class WorldFactory;
};