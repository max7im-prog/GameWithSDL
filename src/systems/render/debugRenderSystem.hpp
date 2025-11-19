#pragma once
#include "debugRenderer.hpp"
#include "gameSystem.hpp"
#include "renderContext.hpp"
#include <entt/entt.hpp>

class DebugRenderSystem :public GameSystem{
public:
  DebugRenderSystem( RenderContext &renderContext);
  ~DebugRenderSystem() = default;
  void update(entt::registry &registry, double secondsPassed);

private:
  DebugRenderSystem() = delete;
  DebugRenderer renderer;
};