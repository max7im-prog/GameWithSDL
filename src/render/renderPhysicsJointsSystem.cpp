#include "renderPhysicsJointsSystem.hpp"

#include "physicsComponents.hpp"
#include "renderUtils.hpp"

RenderPhysicsJointsSystem::RenderPhysicsJointsSystem()
{
}

RenderPhysicsJointsSystem::~RenderPhysicsJointsSystem()
{
}

void RenderPhysicsJointsSystem::update(entt::registry &registry, const RenderContext &renderContext)
{
    auto v = registry.view<PhysicsJoint>();
    for(auto &ent:v){
        auto &comp = v.get<PhysicsJoint>(ent);
        RenderUtils::renderJoint(comp.jointId, renderContext);
    }
}
