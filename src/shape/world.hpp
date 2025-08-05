#pragma once
#include "box2d/id.h"
class World {
public:
  b2WorldId getWorldId() const;

private:
  b2WorldId worldId;
};