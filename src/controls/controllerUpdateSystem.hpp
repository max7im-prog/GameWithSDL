#pragma once
#include <entt/entt.hpp>

// System to update entities with Controller component based on events (player input)
class ControllerUpdateSystem{
    ControllerUpdateSystem();
    ~ControllerUpdateSystem();
    void update(entt::registry& registry);
};