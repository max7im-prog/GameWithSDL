#pragma once

#include "SDL3/SDL.h"
#include "gameSystem.hpp"
#include <entt/entt.hpp>

// System to poll SDL events (mostly player's input)
class PollEventSystem :public GameSystem{
public:
  PollEventSystem();
  ~PollEventSystem() = default;

  void update(entt::registry &registry, double secondsPassed);

private:
  using EventHandler =
      std::function<void(entt::registry &, const SDL_Event &e)>;

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

  static const std::unordered_map<Uint32, EventHandler> eventHandlers;

  void pollEvents(entt::registry& registry);
  void advanceHeld(entt::registry& registry);
};
