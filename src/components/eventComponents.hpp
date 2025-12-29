#pragma once
#include <SDL3/SDL.h>

enum InputState {
  JUST_PRESSED,
  HELD_PRESSED,
  JUST_RELEASED,
  SINGLE_FRAME_EVENT,
  NOT_PRESSED
};

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
