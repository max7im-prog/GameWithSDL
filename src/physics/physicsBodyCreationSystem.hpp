#pragma once

#include <entt/entt.hpp>

#include "physicsObjects.hpp"

class PhysicsBodyCreationSystem{
public:
    PhysicsBodyCreationSystem();
    ~PhysicsBodyCreationSystem();

    void update(entt::registry& registry);
};