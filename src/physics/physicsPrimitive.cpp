#include "physicsPrimitive.hpp"

PhysicsPrimitive::~PhysicsPrimitive() = default;

PhysicsPrimitive::PhysicsPrimitive(entt::registry &registry)
    : RegistryComposite(registry) {}
