#pragma once
#include <entt/entt.hpp>
class CleanupSystem{
public:
    CleanupSystem();
    ~CleanupSystem();

    void update(entt::registry& registry);
};