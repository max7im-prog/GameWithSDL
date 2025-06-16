#pragma once

#include <optional>
#include <SDL3/SDL.h>

struct texture{
    std::optional<SDL_Texture *> texture;
};