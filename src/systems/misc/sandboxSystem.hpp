#pragma once
#include "gameSystem.hpp"
#include "renderContext.hpp"
#include "textureManager.hpp"
#include <entt/entt.hpp>

/**
 * @brief A system designed for testing and debugging purposes. It may do
 * anything and I will frequently radically change it's contents
 *
 */
class SandboxSystem : public GameSystem {
public:
  SandboxSystem();
  ~SandboxSystem();

  void update(entt::registry &registry, const RenderContext &renderContext,
              std::shared_ptr<TextureManager> textureManager,
              double secondsPassed);
};