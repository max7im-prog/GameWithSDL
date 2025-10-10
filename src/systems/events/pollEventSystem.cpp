#include "pollEventSystem.hpp"

#include <SDL3/SDL.h>

#include "SDL3/SDL_events.h"
#include "eventComponents.hpp"
#include "eventUtils.hpp"

const std::unordered_map<Uint32, PollEventSystem::EventHandler>
    PollEventSystem::eventHandlers = {

        {SDL_EVENT_QUIT,
         [](entt::registry &r, const SDL_Event &e) {
           auto ent = r.create();
           auto &quit = r.emplace_or_replace<QuitEvent>(ent);
           quit.event = e;
           r.emplace_or_replace<MarkedForCleanupTag>(ent);
         }},

        {SDL_EVENT_KEY_DOWN,
         [](entt::registry &r, const SDL_Event &e) {
           if (e.key.repeat) {
             return;
           }
           auto foundEvent = EventUtils::getKeyPress(r, e.key.key);
           if (foundEvent) {
             auto ent = *foundEvent;
             auto &kp = r.get<KeyPressEvent>(ent);
             auto &input = r.get<PlayerInput>(ent);
             switch (input.state) {
             case InputState::JUST_PRESSED:
               input.state = InputState::HELD_PRESSED;
               break;
             case InputState::HELD_PRESSED:
               // Do nothing
               break;
             case InputState::JUST_RELEASED:
               // Do nothing
               break;
             case InputState::SINGLE_FRAME_EVENT:
               // Do nothing
               break;
             }
           } else {
             auto ent = r.create();
             auto &input = r.emplace_or_replace<PlayerInput>(ent);
             input.state = InputState::JUST_PRESSED;
             auto &kp = r.emplace_or_replace<KeyPressEvent>(ent);
             kp.event = e;
           }
         }},

        {SDL_EVENT_KEY_UP,
         [](entt::registry &r, const SDL_Event &e) {
           if (e.key.repeat) {
             return;
           }
           auto foundEvent = EventUtils::getKeyPress(r, e.key.key);
           if (foundEvent) {
             auto ent = *foundEvent;
             auto &kp = r.get<KeyPressEvent>(ent);
             auto &input = r.get<PlayerInput>(ent);
             switch (input.state) {
             case InputState::JUST_PRESSED:
               input.state = InputState::SINGLE_FRAME_EVENT;
               r.emplace_or_replace<MarkedForCleanupTag>(ent);
               break;
             case InputState::HELD_PRESSED:
               input.state = InputState::JUST_RELEASED;
               r.emplace_or_replace<MarkedForCleanupTag>(ent);
               break;
             case InputState::JUST_RELEASED:
               // Do nothing
               break;
             case InputState::SINGLE_FRAME_EVENT:
               // Do nothing
               break;
             }
           } else {
             // Do nothing
           }
         }},

        {SDL_EVENT_MOUSE_BUTTON_DOWN,
         [](entt::registry &r, const SDL_Event &e) {
           auto foundEvent = EventUtils::getButtonPress(r, e.button.button);
           if (foundEvent) {
             auto ent = *foundEvent;
             auto &kp = r.get<KeyPressEvent>(ent);
             auto &input = r.get<PlayerInput>(ent);
             switch (input.state) {
             case InputState::JUST_PRESSED:
               input.state = InputState::HELD_PRESSED;
               break;
             case InputState::HELD_PRESSED:
               // Do nothing
               break;
             case InputState::JUST_RELEASED:
               // Do nothing
               break;
             case InputState::SINGLE_FRAME_EVENT:
               // Do nothing
               break;
             }
           } else {
             auto ent = r.create();
             auto &input = r.emplace_or_replace<PlayerInput>(ent);
             input.state = InputState::JUST_PRESSED;
             auto &kp = r.emplace_or_replace<KeyPressEvent>(ent);
             kp.event = e;
           }
         }},
        {SDL_EVENT_MOUSE_BUTTON_UP,
         [](entt::registry &r, const SDL_Event &e) {
           auto foundEvent = EventUtils::getButtonPress(r, e.button.button);
           if (foundEvent) {
             auto ent = *foundEvent;
             auto &bp = r.get<ButtonPressEvent>(ent);
             auto &input = r.get<PlayerInput>(ent);
             switch (input.state) {
             case InputState::JUST_PRESSED:
               input.state = InputState::SINGLE_FRAME_EVENT;
               r.emplace_or_replace<MarkedForCleanupTag>(ent);
               break;
             case InputState::HELD_PRESSED:
               input.state = InputState::JUST_RELEASED;
               r.emplace_or_replace<MarkedForCleanupTag>(ent);
               break;
             case InputState::JUST_RELEASED:
               // Do nothing
               break;
             case InputState::SINGLE_FRAME_EVENT:
               // Do nothing
               break;
             }
           } else {
             // Do nothing
           }
         }},
        {SDL_EVENT_MOUSE_WHEEL, [](entt::registry &r, const SDL_Event &e) {}},

};

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
    } else if (event.type == SDL_EVENT_KEY_DOWN) {
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

void PollEventSystem::pollEvents(entt::registry &registry) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (PollEventSystem::eventHandlers.contains(event.type)) {
      (PollEventSystem::eventHandlers.at(event.type))(registry, event);
    }
  }
}

void PollEventSystem::advanceHeld(entt::registry &registry) {
  {
    auto v = registry.view<KeyPressEvent, PlayerInput>();
    for (auto &ent : v) {
      auto &input = registry.get<PlayerInput>(ent);
      if (input.state == InputState::JUST_PRESSED) {
        input.state = InputState::HELD_PRESSED;
      }
    }
  }
  {
    auto v = registry.view<ButtonPressEvent, PlayerInput>();
    for (auto &ent : v) {
      auto &input = registry.get<PlayerInput>(ent);
      if (input.state == InputState::JUST_PRESSED) {
        input.state = InputState::HELD_PRESSED;
      }
    }
  }
}