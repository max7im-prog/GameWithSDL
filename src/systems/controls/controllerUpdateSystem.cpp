#include "controllerUpdateSystem.hpp"

#include <iostream>

#include "controlComponents.hpp"
#include "eventComponents.hpp"

ControllerUpdateSystem::ControllerUpdateSystem()
{
}

ControllerUpdateSystem::~ControllerUpdateSystem()
{
}

void ControllerUpdateSystem::update(entt::registry &registry)
{
    
    // entt::entity ent;
    // {
    //     auto v = registry.view<Controller>();
    //     if (v.size() != 0)
    //     {
    //         for (auto i : v)
    //         {
    //             ent = i;
    //             break;
    //         }
    //     }
    //     else
    //     {
    //         ent = registry.create();
    //         registry.emplace_or_replace<Controller>(ent);
    //     }
    // }

    // auto &controller = registry.get<Controller>(ent);
    // {
    //     auto view = registry.view<const PlayerInput, KeyPress>();
    //     for (auto [ent, press] : view.each())
    //     {
    //         if (press.key == SDLK_RIGHT && press.pressed)
    //         {
    //             controller.moveDirection = {1, 0};
    //             // std::cout << "right" << std::endl;
    //         }
    //         else if (press.key == SDLK_LEFT && press.pressed)
    //         {
    //             controller.moveDirection = {-1, 0};
    //             // std::cout << "left" << std::endl;
    //         }else if (press.key == SDLK_RIGHT && !press.pressed)
    //         {
    //             controller.moveDirection = {0, 0};
    //             // std::cout << "right" << std::endl;
    //         }
    //         else if (press.key == SDLK_LEFT && !press.pressed)
    //         {
    //             controller.moveDirection = {0, 0};
    //             // std::cout << "left" << std::endl;
    //         }
    //     }
    // }
}