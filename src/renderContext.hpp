#ifndef RENDERCONTEXT_HPP
#define RENDERCONTEXT_HPP
#include <SDL3/SDL.h>

class RenderContext
{
public:
    SDL_Renderer *renderer;
    SDL_Window *window;
};

#endif //RENDERCONTEXT_HPP