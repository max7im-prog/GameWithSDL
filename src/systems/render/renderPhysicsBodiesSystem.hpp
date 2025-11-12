#pragma once
#include <entt/entt.hpp>

#include "gameSystem.hpp"
#include "renderContext.hpp"


class RenderPhysicsBodiesSystem:public GameSystem{
public:
    RenderPhysicsBodiesSystem();
    ~RenderPhysicsBodiesSystem();

    void update(entt::registry &registry, const RenderContext &renderContext);
};