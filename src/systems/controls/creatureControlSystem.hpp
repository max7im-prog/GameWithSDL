#pragma once
#include <entt/entt.hpp>

// System to update creatures with controllerattached to them.
class CreatureControlSystem{
public:
    CreatureControlSystem() = default;
    ~CreatureControlSystem() = default;
    void update(entt::registry& registry);
};