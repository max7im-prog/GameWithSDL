#pragma once
#include "SDL3/SDL_events.h"
#include "eventUtils.hpp"
#include <SDL3/SDL.h>
#include <utility>

struct PlayerInput {
  InputState state;
};

struct QuitEvent {};

struct KeyPressEvent {
  SDL_Event event;
};

struct ButtonPressEvent {
  SDL_Event event;
};