#pragma once
#include <entt/entt.hpp>

// System to update creatures based on a Controller
class CreatureControlSystem{
public:
    CreatureControlSystem();
    ~CreatureControlSystem();
    void update(entt::registry& registry);
};