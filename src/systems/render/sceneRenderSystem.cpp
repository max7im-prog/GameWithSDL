
#include "sceneRenderSystem.hpp"
#include "creatureComponents.hpp"
#include "gameSystem.hpp"
#include "terrainComponents.hpp"

SceneRenderSystem::SceneRenderSystem(entt::registry &registry,RenderContext &renderContext)
    : GameSystem("SceneRenderSystem"), renderer(registry,renderContext) {}

void SceneRenderSystem::update(entt::registry &registry, double secondsPassed) {
  if (!shouldRun(secondsPassed)) {
    return;
  }
  {
    auto v = registry.view<PhysicsCreature>();
    for (auto &ent : v) {
      auto &comp = v.get<PhysicsCreature>(ent);
      if (comp.creature != nullptr) {
        comp.creature->accept(renderer);
      }
    }
  }
  {
    auto v = registry.view<PhysicsTerrain>();
    for (auto &ent : v) {
      auto &comp = v.get<PhysicsTerrain>(ent);
      if (comp.terrain != nullptr) {
        comp.terrain->accept(renderer);
      }
    }
  }
}
