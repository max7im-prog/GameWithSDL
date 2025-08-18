#pragma once
#include <SDL3/SDL.h>

class RenderContext
{
public:
    SDL_Renderer *renderer;
    SDL_Window *window;

    // Position of bottom left point of a window in box2d meters
    float x,y;

    // Width and height of a window in pixels
    float w,h;

    // Conversion from box2d meters to pixels
    float pixelToMeterRatio;
};
