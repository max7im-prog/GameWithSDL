#pragma once

#include "SDL3/SDL_keycode.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_stdinc.h>
#include <entt/entt.hpp>

// System to poll SDL events (mostly player's input)
class PollEventSystem {
public:
  PollEventSystem() = default;
  ~PollEventSystem() = default;

  void update(entt::registry &registry);

private:
  // For key presses
  std::map<SDL_Keycode, entt::entity> keyMap = {};
  std::unordered_set<SDL_Keycode> justPressedKeys = {};
  std::unordered_set<SDL_Keycode> heldPressedKeys = {};
  std::unordered_set<SDL_Keycode> justReleasedKeys = {};

  // For button presses
  std::map<Uint8, entt::entity> buttonMap = {};
  std::unordered_set<Uint8> justPressedButtons = {};
  std::unordered_set<Uint8> heldPressedButtons = {};
  std::unordered_set<Uint8> justReleasedButtons = {};

  std::vector<entt::entity> singleFrameEvents = {};
};
