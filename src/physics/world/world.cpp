#include "world.hpp"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include "registryObject.hpp"

b2WorldId World::getWorldId() const { return worldId; }

World::World(entt::registry &registry)
    : RegistryObject(registry), worldId(b2_nullWorldId) {}

World::~World() {
  if (b2World_IsValid(worldId)) {
    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
  }
}

void World::step(float dt){
  b2World_Step(worldId, dt, substepCount);
}