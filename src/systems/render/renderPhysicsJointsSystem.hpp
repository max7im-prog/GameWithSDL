#include <entt/entt.hpp>
#include "renderContext.hpp"

class RenderPhysicsJointsSystem{
public:
    RenderPhysicsJointsSystem();
    ~RenderPhysicsJointsSystem();

    void update(entt::registry &registry, const RenderContext &renderContext);
};