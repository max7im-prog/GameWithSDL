#pragma once
#include "registryObject.hpp"
#include "world.hpp"
#include <box2d/box2d.h>

struct TerrainConfig {
  b2Vec2 position;
  static b2Filter defaultFilter();
};

class Terrain : public RegistryObject {
public:
  virtual void update(float dt) override;
  virtual ~Terrain() = 0;

protected:
  Terrain(entt::registry &registry, const std::shared_ptr<World> world);

  const std::shared_ptr<World> world;

private:
  Terrain() = delete;
  Terrain(Terrain &other) = delete;
  Terrain(Terrain &&other) = delete;
  Terrain &operator=(Terrain &other) = delete;
  Terrain &operator=(Terrain &&other) = delete;

  friend class TerrainFactory;
};