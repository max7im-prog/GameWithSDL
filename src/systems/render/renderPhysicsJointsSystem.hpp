#pragma once
#include <entt/entt.hpp>
#include "gameSystem.hpp"
#include "renderContext.hpp"

class RenderPhysicsJointsSystem:public GameSystem{
public:
    RenderPhysicsJointsSystem();
    ~RenderPhysicsJointsSystem();

    void update(entt::registry &registry, const RenderContext &renderContext);
};