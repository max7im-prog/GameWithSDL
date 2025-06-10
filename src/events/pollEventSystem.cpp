#include "pollEventSystem.hpp"

#include <SDL3/SDL.h>

#include "events.hpp"

PollEventSystem::PollEventSystem()
{
}

PollEventSystem::~PollEventSystem()
{
}

void PollEventSystem::update(entt::registry &registry)
{
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_EVENT_QUIT){
            auto ent = registry.create();
            registry.emplace<QuitEvent>(ent);
        }
    }
}
