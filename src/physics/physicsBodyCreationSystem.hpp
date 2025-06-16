#pragma once

#include <entt/entt.hpp>

#include "physicsComponents.hpp"

// System for creating PhysicsBodies from PendingPhysicsBodies
class PhysicsBodyCreationSystem{
public:
    PhysicsBodyCreationSystem();
    ~PhysicsBodyCreationSystem();

    void update(entt::registry& registry);
};