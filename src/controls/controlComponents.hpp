#pragma once
#include <box2d/box2d.h>
#include <entt/entt.hpp>

struct PlayerControlled {};

struct Controller {
  entt::entity creature;
  b2Vec2 moveDir;
  float moveIntensity;
  bool jump = false;
  bool attack = false;
  bool defend = false;
  struct AimContext{
    b2Vec2 worldPoint;
    bool aim;
    bool update;
  }aimContext;
  struct LookContext{
    b2Vec2 worldPoint;
    bool look;
    bool update;
  }lookContext;
};