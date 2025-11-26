
#include "sandboxSystem.hpp"
#include "gameSystem.hpp"

SandboxSystem::SandboxSystem() : GameSystem("SandboxSystem") {}

SandboxSystem::~SandboxSystem() {}

void SandboxSystem::update(entt::registry &registry,
                           const RenderContext &renderContext,
                           std::shared_ptr<TextureManager> textureManager,
                           double secondsPassed) {
  if (!shouldRun(secondsPassed)) {
    return;
  }
  
}
