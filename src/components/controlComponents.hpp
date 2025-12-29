#pragma once
#include "creature.hpp"
#include "eventComponents.hpp"
#include <box2d/box2d.h>
#include <entt/entt.hpp>

struct PlayerControlled {};

struct Controller {
  using ActionState = struct ActionState {
    InputState _inputState = InputState::NOT_PRESSED;
  };

  entt::entity _creature = entt::null;
  std::map<CreatureAction, ActionState> _actions;
  b2Vec2 _lookWorldPoint = {0, 0};
  b2Vec2 _moveDir = {0, 0};
};
