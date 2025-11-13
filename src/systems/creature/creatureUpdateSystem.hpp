#pragma once
#include "gameSystem.hpp"
#include <entt/entt.hpp>

class CreatureUpdateSystem:public GameSystem{
public:
    CreatureUpdateSystem();
    ~CreatureUpdateSystem();

    void update(entt::registry& registry,double secondsPassed);
};