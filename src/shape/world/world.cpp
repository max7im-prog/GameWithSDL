#include "world.hpp"
#include "box2d/id.h"

b2WorldId World::getWorldId() const { return worldId; }

World::World() {}