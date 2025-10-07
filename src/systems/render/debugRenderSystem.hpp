
#include "debugRenderer.hpp"
#include "renderContext.hpp"
#include <entt/entt.hpp>

class DebugRenderSystem {
public:
  DebugRenderSystem( RenderContext &renderContext);
  ~DebugRenderSystem() = default;
  void update(entt::registry &registry);

private:
  DebugRenderSystem() = delete;
  DebugRenderer renderer;
};