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
             auto &bp = r.get<ButtonPressEvent>(ent);
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
             auto &bp = r.emplace_or_replace<ButtonPressEvent>(ent);
             bp.event = e;
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
  advanceHeld(registry);
  pollEvents(registry);
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