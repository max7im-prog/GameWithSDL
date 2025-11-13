#pragma once
#include "gameSystem.hpp"
#include <entt/entt.hpp>
class CleanupSystem:public GameSystem{
public:
    CleanupSystem();
    ~CleanupSystem();

    void update(entt::registry& registry, double secondsPassed);
};