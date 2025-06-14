#ifndef RENDERCONTEXT_HPP
#define RENDERCONTEXT_HPP
#include <SDL3/SDL.h>

class RenderContext
{
public:
    SDL_Renderer *renderer;
    SDL_Window *window;

    // Position of bottom left point of a window in box2d meters
    double x,y;

    // Width and height of a window in pixels
    int w,h;

    // Conversion from box2d meters to pixels
    float pixelToMeterRatio;
};

#endif //RENDERCONTEXT_HPP