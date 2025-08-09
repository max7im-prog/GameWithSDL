#include "basicWorld.hpp"
#include "box2d/box2d.h"
#include <stdexcept>

BasicWorld::BasicWorld() {
  auto wrldDef = b2DefaultWorldDef();
  wrldDef.gravity = (b2Vec2){0.0f, -10.0f};
  worldId = b2CreateWorld(&wrldDef);
  if(!b2World_IsValid(worldId)){
    throw std::runtime_error("Failed to create world");
  }
}

BasicWorld::~BasicWorld(){
    b2DestroyWorld(worldId);
}