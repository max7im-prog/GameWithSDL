#include "pollEventSystem.hpp"

#include <SDL3/SDL.h>

#include "eventComponents.hpp"

#include <iostream>

PollEventSystem::PollEventSystem()
{
}

PollEventSystem::~PollEventSystem()
{
}

void PollEventSystem::update(entt::registry &registry)
{
    // Remove all player input from previous iteration
    auto view = registry.view<PlayerInput>();
    registry.destroy(view.begin(), view.end());


    // Handle player input
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // std::cout <<"polled event" << std::endl;
        if (event.type == SDL_EVENT_QUIT)
        {
            auto ent = registry.create();
            registry.emplace<QuitEvent>(ent);
            registry.emplace<PlayerInput>(ent);
        }
        else if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
        {
            auto ent = registry.create();
            registry.emplace<PlayerInput>(ent);

            auto &kp = registry.emplace<KeyPress>(ent);
            kp.key = event.key.key; // SDL_Keycode
            kp.pressed = (event.type == SDL_EVENT_KEY_DOWN);
            // if(kp.pressed){
            //     std::cout <<"Pressed key " <<kp.key << std::endl; 
            // }else{
            //     std::cout <<"Released key " <<kp.key << std::endl; 
            // }
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            auto ent = registry.create();
            registry.emplace<PlayerInput>(ent);

            auto &bp = registry.emplace<ButtonPress>(ent);
            bp.button = event.button.button; // SDL button code
            bp.pressed = (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
            bp.location = {event.button.x,event.button.y};

            // if(bp.pressed){
            //     std::cout <<"Pressed button " <<bp.button << " at {" <<bp.location.first <<","<<bp.location.second <<"}"  << std::endl; 
            // }else{
            //     std::cout <<"Released button " <<bp.button << " at {" <<bp.location.first <<","<<bp.location.second <<"}"<< std::endl; 
            // }
        }
    }
}
