#include "renderBackgroundSystem.hpp"

#include <SDL3/SDL.h>

#include <iostream>

#include "renderUtils.hpp"

RenderBackgroundSystem::RenderBackgroundSystem():GameSystem("RenderBackgroundSystem")
{
}

RenderBackgroundSystem::~RenderBackgroundSystem()
{
}

void RenderBackgroundSystem::update(entt::registry &registry,const RenderContext &renderContext)
{
  auto renderer = renderContext.getSDLRenderer();
    if(!SDL_SetRenderDrawColor(renderer,255,255,255,255)){
        std::cerr <<"failed to set render color" <<std::endl;
    }
    if(!SDL_RenderClear(renderer)){
        std::cerr <<"failed to render clear" <<std::endl;
    }
    
}
