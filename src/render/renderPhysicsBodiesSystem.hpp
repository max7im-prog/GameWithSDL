#pragma once
#include <entt/entt.hpp>


class RenderPhysicsBodiesSystem{
public:
    RenderPhysicsBodiesSystem();
    ~RenderPhysicsBodiesSystem();

    void update(entt::registry &registry);
};