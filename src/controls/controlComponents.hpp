#pragma once
#include <box2d/box2d.h>
#include <entt/entt.hpp>

struct PlayerControlled {};

struct Controller {
  entt::entity creature = entt::null;

  bool jump = false;
  bool attack = false;
  bool defend = false;
  struct AimContext {
    b2Vec2 worldPoint = {0, 0};
    bool aim = false;
    bool update = false;
  } aimContext;
  struct LookContext {
    b2Vec2 worldPoint = {0, 0};
    bool look = false;
    bool update = false;
  } lookContext;
  struct MoveContext {
    b2Vec2 moveDir = {0, 0};
    float moveIntensity = 0;
    bool update = false;
  } moveContext;
};