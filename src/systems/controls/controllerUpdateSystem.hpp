#pragma once
#include "renderContext.hpp"
#include <entt/entt.hpp>

// System to update entities with Controller component based on events (player input)
class ControllerUpdateSystem{
public:
    ControllerUpdateSystem();
    ~ControllerUpdateSystem();
    void update(entt::registry& registry,const RenderContext &renderContext);
};