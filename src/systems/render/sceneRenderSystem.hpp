
#pragma once
#include "gameSystem.hpp"
#include "renderContext.hpp"
#include "sceneRenderer.hpp"
#include <entt/entt.hpp>

class SceneRenderSystem : public GameSystem {
public:
  SceneRenderSystem(entt::registry &registry, RenderContext &renderContext);
  ~SceneRenderSystem() = default;
  void update(entt::registry &registry, double secondsPassed);

private:
  SceneRenderSystem() = delete;
  SceneRenderer renderer;
};
