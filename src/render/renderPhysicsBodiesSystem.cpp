#include "renderPhysicsBodiesSystem.hpp"

#include "physicsComponents.hpp"
#include "renderUtils.hpp"

RenderPhysicsBodiesSystem::RenderPhysicsBodiesSystem() {}

RenderPhysicsBodiesSystem::~RenderPhysicsBodiesSystem() {}

void RenderPhysicsBodiesSystem::update(entt::registry &registry,
                                       const RenderContext &renderContext) {
  auto v = registry.view<PhysicsBody>();
  for (auto &ent : v) {
    auto &comp = v.get<PhysicsBody>(ent);
    if (comp.shape != nullptr) {
      RenderUtils::renderShape(comp.shape->getShapeId(), renderContext);
    }
  }
}