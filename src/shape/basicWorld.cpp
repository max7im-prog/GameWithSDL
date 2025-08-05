#include "basicWorld.hpp"
#include "box2d/box2d.h"

BasicWorld::BasicWorld() {
  auto wrldDef = b2DefaultWorldDef();
  wrldDef.gravity = (b2Vec2){0.0f, -10.0f};
  worldId = b2CreateWorld(&wrldDef);
}

BasicWorld::~BasicWorld(){
    b2DestroyWorld(worldId);
}