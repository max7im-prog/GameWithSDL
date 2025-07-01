#pragma once
#include <entt/entt.hpp>

class CreatureControlSystem{
public:
    CreatureControlSystem();
    ~CreatureControlSystem();
    void update(entt::registry& registry);
};