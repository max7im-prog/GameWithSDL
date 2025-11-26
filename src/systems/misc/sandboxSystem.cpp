
#include "sandboxSystem.hpp"
#include "SDL3/SDL_rect.h"
#include "commonClasses.hpp"
#include "gameSystem.hpp"
#include "renderUtils.hpp"

SandboxSystem::SandboxSystem() : GameSystem("SandboxSystem") {
  _currentTransform._scaleX = 4;
  _currentTransform._scaleY = 2;
  _currentTransform._flipY = true;
  _currentTransform._flipX = true;
}

SandboxSystem::~SandboxSystem() {}

void SandboxSystem::update(entt::registry &registry,
                           const RenderContext &renderContext,
                           std::shared_ptr<TextureManager> textureManager,
                           double secondsPassed) {
  if (!shouldRun(secondsPassed)) {
    return;
  }

  auto texture = textureManager->getTexture("res/textures/test/test_2.png");
  if (!texture) {
    return;
  }

  SDL_FRect region;
  region.x = 0;
  region.y = 0;
  region.w = texture->w;
  region.h = texture->h;

  _currentTransform._relativeRot =
      b2MulRot(_currentTransform._relativeRot, b2MakeRot(B2_PI / 100));
  // _currentTransform._relativePos= b2Add(_currentTransform._relativePos,{0.1,0.1});

  RenderUtils::renderTexture(texture.get(), region, 10, 10, _currentTransform,
                             renderContext);
}
