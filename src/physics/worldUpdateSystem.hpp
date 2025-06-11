#pragma once
#include <entt/entt.hpp>
#include <box2d/box2d.h>

class WorldUpdateSystem{
public:

    WorldUpdateSystem();
    ~WorldUpdateSystem();

    void update(entt::registry& registry, int FPS);

};