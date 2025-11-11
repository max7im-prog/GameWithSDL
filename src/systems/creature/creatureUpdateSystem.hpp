#pragma once
#include <entt/entt.hpp>

class CreatureUpdateSystem{
public:
    CreatureUpdateSystem();
    ~CreatureUpdateSystem();

    void update(entt::registry& registry,int TPS);
};