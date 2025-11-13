#pragma once
#include "gameSystem.hpp"
#include "renderContext.hpp"
#include <entt/entt.hpp>

// System to update entities with Controller component based on events (player input)
class ControllerUpdateSystem:public GameSystem{
public:
    ControllerUpdateSystem();
    ~ControllerUpdateSystem() = default;
    void update(entt::registry& registry,const RenderContext &renderContext, double secondsPassed);
};