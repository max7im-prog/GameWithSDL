#pragma once
#include <SDL3/SDL.h>

struct QuitEvent{};

struct PlayerInput{};


struct KeyPress {
    SDL_Keycode key;  // e.g. SDLK_SPACE, SDLK_a, SDLK_RETURN
    bool pressed;     // true if key down, false if key up
};

struct ButtonPress {
    uint8_t button;   // SDL button code, e.g. SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT
    bool pressed;     // true if pressed, false if released
};