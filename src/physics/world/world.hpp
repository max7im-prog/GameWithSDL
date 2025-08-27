#pragma once
#include "box2d/box2d.h"
#include "registryComposite.hpp"
#include <SDL3/SDL_stdinc.h>

struct WorldConfig {
  Uint32 substepCount = 4;
};

class World : public RegistryComposite {
public:
  b2WorldId getWorldId() const;
  virtual ~World();
  void step(float dt);

protected:
  World(entt::registry &registry);
  b2WorldId worldId;
  Uint32 substepCount = 1;

private:
  friend class WorldFactory;
};