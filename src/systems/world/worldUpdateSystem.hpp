#pragma once
#include "gameSystem.hpp"
#include <box2d/box2d.h>
#include <entt/entt.hpp>

// System that calls step() in every physics world in a registry
class WorldUpdateSystem :public GameSystem{
public:
  WorldUpdateSystem();
  ~WorldUpdateSystem();

  void update(entt::registry &registry, int TPS);
};