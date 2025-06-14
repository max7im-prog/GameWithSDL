#pragma once
#include <entt/entt.hpp>

#include "renderContext.hpp"


class RenderPhysicsBodiesSystem{
public:
    RenderPhysicsBodiesSystem();
    ~RenderPhysicsBodiesSystem();

    void update(entt::registry &registry, const RenderContext &renderContext);
};