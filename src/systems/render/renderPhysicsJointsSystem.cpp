#include "renderPhysicsJointsSystem.hpp"

#include "physicsComponents.hpp"
#include "renderUtils.hpp"

RenderPhysicsJointsSystem::RenderPhysicsJointsSystem() :GameSystem("RenderPhysicsJointSystem"){}

RenderPhysicsJointsSystem::~RenderPhysicsJointsSystem() {}

void RenderPhysicsJointsSystem::update(entt::registry &registry,
                                       const RenderContext &renderContext) {
  // auto v = registry.view<PhysicsJoint>();
  // for (auto &ent : v) {
  //   auto &comp = v.get<PhysicsJoint>(ent);
  //   if (comp.joint != nullptr) {
  //     RenderUtils::renderJoint(comp.joint->getJointId(), renderContext);
  //   }
  // }
}
