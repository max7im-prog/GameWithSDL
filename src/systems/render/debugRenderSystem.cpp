#include "debugRenderSystem.hpp"
#include "physicsComponents.hpp"

DebugRenderSystem::DebugRenderSystem(RenderContext &renderContext)
    : renderer(renderContext) {}

void DebugRenderSystem::update(entt::registry &registry) {
  auto v = registry.view<PhysicsShape>();
  for (auto &ent : v) {
    auto &comp = v.get<PhysicsShape>(ent);
    if (comp.shape != nullptr) {
      comp.shape->accept(renderer);
    }
  }
}