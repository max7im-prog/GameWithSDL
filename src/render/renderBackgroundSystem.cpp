#include "renderBackgroundSystem.hpp"

#include <SDL3/SDL.h>

#include <iostream>

RenderBackgroundSystem::RenderBackgroundSystem()
{
}

RenderBackgroundSystem::~RenderBackgroundSystem()
{
}

void RenderBackgroundSystem::update(entt::registry &registry,const RenderContext &renderContext)
{
    if(!SDL_SetRenderDrawColor(renderContext.renderer,255,255,255,255)){
        std::cerr <<"failed to set render color" <<std::endl;
    }
    if(!SDL_RenderClear(renderContext.renderer)){
        std::cerr <<"failed to render clear" <<std::endl;
    }
}
