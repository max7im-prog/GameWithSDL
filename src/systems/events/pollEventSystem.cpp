#include "pollEventSystem.hpp"

#include <SDL3/SDL.h>

#include "SDL3/SDL.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL.h"
#include "eventComponents.hpp"
#include "eventUtils.hpp"


void PollEventSystem::update(entt::registry &registry) {

  // Update state of key presses from previous frame
  {
    std::vector<SDL_Keycode> toRemove = {};
    for (auto &[keycode, ent] : keyMap) {
      auto &input = registry.get<PlayerInput>(ent);
      auto &kp = registry.get<KeyPressEvent>(ent);
      switch (input.state) {
      case InputState::JUST_PRESSED:
        // Move to heldPressed
        justPressedKeys.erase(keycode);
        heldPressedKeys.insert(keycode);
        input.state = InputState::HELD_PRESSED;

        break;
      case InputState::HELD_PRESSED:
        // Do nothing
        break;
      case InputState::JUST_RELEASED:
        // Remove
        toRemove.push_back(keycode);

        break;
      case InputState::SINGLE_FRAME_EVENT:
        // Remove
        toRemove.push_back(keycode);

        break;
      }
    }
    for (auto keycode : toRemove) {
      if (registry.valid(keyMap[keycode])) {
        registry.destroy(keyMap[keycode]);
      }
      keyMap.erase(keycode);
      justPressedKeys.erase(keycode);
      justReleasedKeys.erase(keycode);
      heldPressedKeys.erase(keycode);
    }
    justPressedKeys = {};
    justReleasedKeys = {};
  }

  // Update state of button presses from previous frame
  {
    std::vector<Uint8> toRemove = {};
    for (auto &[buttoncode, ent] : buttonMap) {
      auto &input = registry.get<PlayerInput>(ent);
      auto &bp = registry.get<ButtonPressEvent>(ent);
      switch (input.state) {
      case InputState::JUST_PRESSED:
        // Move to heldPressed
        justPressedButtons.erase(buttoncode);
        heldPressedButtons.insert(buttoncode);
        input.state = InputState::HELD_PRESSED;

        break;
      case InputState::HELD_PRESSED:
        // Do nothing
        break;
      case InputState::JUST_RELEASED:
        // Remove
        toRemove.push_back(buttoncode);

        break;
      case InputState::SINGLE_FRAME_EVENT:
        // Remove
        toRemove.push_back(buttoncode);

        break;
      }
    }
    for (auto buttoncode : toRemove) {
      if (registry.valid(buttonMap[buttoncode])) {
        registry.destroy(buttonMap[buttoncode]);
      }
      buttonMap.erase(buttoncode);
      justPressedButtons.erase(buttoncode);
      justReleasedButtons.erase(buttoncode);
      heldPressedButtons.erase(buttoncode);
    }
    justPressedButtons = {};
    justReleasedButtons = {};
  }

  // Handle player input
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    entt::entity ent;
    if (event.type == SDL_EVENT_QUIT) {
      ent = registry.create();
      registry.emplace<QuitEvent>(ent);
      auto &elem = registry.emplace<PlayerInput>(ent);
      elem.state = InputState::SINGLE_FRAME_EVENT;
      singleFrameEvents.push_back(ent);
    }
    else if (event.type == SDL_EVENT_KEY_DOWN) {
      auto keycode = event.key.key;
      if (!keyMap.contains(keycode)) {
        ent = registry.create();
        auto &input = registry.emplace_or_replace<PlayerInput>(ent);
        input.state = InputState::JUST_PRESSED;
        auto &kp = registry.emplace_or_replace<KeyPressEvent>(ent);
        kp.event = event;
        justPressedKeys.insert(keycode);
        keyMap[keycode] = ent;
      }
    } else if (event.type == SDL_EVENT_KEY_UP) {
      auto keycode = event.key.key;
      if (keyMap.contains(keycode)) {
        ent = keyMap[keycode];
        auto &input = registry.get<PlayerInput>(ent);
        if (justPressedKeys.contains(keycode)) {
          // Press and release on the same tick
          input.state = SINGLE_FRAME_EVENT;

        } else if (heldPressedKeys.contains(keycode)) {
          // Released after being held
          input.state = JUST_RELEASED;
        }
      }
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
      auto buttoncode = event.button.button;
      if (!buttonMap.contains(buttoncode)) {
        ent = registry.create();
        auto &input = registry.emplace_or_replace<PlayerInput>(ent);
        input.state = InputState::JUST_PRESSED;
        auto &bp = registry.emplace_or_replace<ButtonPressEvent>(ent);
        bp.event = event;
        justPressedButtons.insert(buttoncode);
        buttonMap[buttoncode] = ent;
      }
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
      auto buttoncode = event.button.button;
      if (buttonMap.contains(buttoncode)) {
        ent = buttonMap[buttoncode];
        auto &input = registry.get<PlayerInput>(ent);
        if (justPressedButtons.contains(buttoncode)) {
          // Press and release on the same tick
          input.state = SINGLE_FRAME_EVENT;
        } else if (heldPressedButtons.contains(buttoncode)) {
          // Released after being held
          input.state = JUST_RELEASED;
        }
      }
    }
  }
}
