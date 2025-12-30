#pragma once
#include "commonClasses.hpp"
#include "gameSystem.hpp"
#include "renderContext.hpp"
#include "renderUpdater.hpp"
#include "textureManager.hpp"
#include <entt/entt.hpp>
#include <memory>

/**
 * @brief A system designed for testing and debugging purposes. It may do
 * anything and I will frequently radically change it's contents.
 *
 */
class SandboxSystem : public GameSystem {
public:
  SandboxSystem();
  ~SandboxSystem();

  void update(entt::registry &registry, const RenderContext &renderContext,
              std::shared_ptr<TextureManager> textureManager,
              double secondsPassed);
  void setCreature(std::weak_ptr<DemoCreature> cr);

private:
  Common::Transform _currentTransform;
  std::shared_ptr<RenderUpdater> _renderUpdater = nullptr;
  std::weak_ptr<DemoCreature> _creature;
};
