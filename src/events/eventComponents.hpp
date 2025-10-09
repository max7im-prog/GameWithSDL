#pragma once
#include "eventUtils.hpp"
#include <SDL3/SDL.h>

struct PlayerInput {
  InputState state;
};
struct QuitEvent {
  SDL_Event event;
};
struct KeyPressEvent {
  SDL_Event event;
};
struct ButtonPressEvent {
  SDL_Event event;
};
struct MouseMotionEvent {
  SDL_Event event;
};
struct MouseWheelEvent {
  SDL_Event event;
};
struct MarkedForCleanupTag {};