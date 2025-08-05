#pragma once
#include "box2d/box2d.h"
class World {
public:
  b2WorldId getWorldId() const;

protected:
  World();
  World(World &other) = delete;
  World(World &&other) = delete;
  World &operator=(World &other) = delete;
  World &operator=(World &&other) = delete;
  virtual ~World() = default;

  b2WorldId worldId;

};