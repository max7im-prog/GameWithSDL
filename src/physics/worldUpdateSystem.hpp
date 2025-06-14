#pragma once
#include <entt/entt.hpp>
#include <box2d/box2d.h>

// System that calls for step() in every physics world in a registry
class WorldUpdateSystem{
public:

    WorldUpdateSystem();
    ~WorldUpdateSystem();

    void update(entt::registry& registry, int FPS);

};