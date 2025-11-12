#include "gameSystem.hpp"
#include "renderContext.hpp"
#include <entt/entt.hpp>

class RenderBackgroundSystem : public GameSystem {
public:
  RenderBackgroundSystem();
  ~RenderBackgroundSystem();

  void update(entt::registry &registry, const RenderContext &renderContext);
};