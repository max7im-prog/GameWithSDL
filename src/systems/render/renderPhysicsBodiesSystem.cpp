#include "renderPhysicsBodiesSystem.hpp"

#include "gameSystem.hpp"
#include "physicsComponents.hpp"
#include "renderUtils.hpp"

RenderPhysicsBodiesSystem::RenderPhysicsBodiesSystem() :GameSystem("RenderPhysicsBodiesSystem"){}

RenderPhysicsBodiesSystem::~RenderPhysicsBodiesSystem() {}

void RenderPhysicsBodiesSystem::update(entt::registry &registry,
                                       const RenderContext &renderContext) {
  // auto v = registry.view<PhysicsShape>();
  // for (auto &ent : v) {
  //   auto &comp = v.get<PhysicsShape>(ent);
  //   if (comp.shape != nullptr) {
  //     RenderUtils::renderShape(comp.shape->getShapeId(), renderContext);
  //   }
  // }
}