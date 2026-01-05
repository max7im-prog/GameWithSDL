
#include "sandboxSystem.hpp"
#include "SDL3/SDL_rect.h"
#include "box2d/math_functions.h"
#include "commonClasses.hpp"
#include "creatureComponents.hpp"
#include "gameSystem.hpp"
#include "physicsComponents.hpp"
#include "renderComponents.hpp"
#include "renderUtils.hpp"
#include <iostream>

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

  //------------------------------------------TREE-----------------------------------------
  auto texture = textureManager->getTexture("res/textures/test/test_2.png");
  if (!texture) {
    return;
  }

  SDL_FRect region;
  region.x = 0;
  region.y = 0;
  region.w = texture->w;
  region.h = texture->h;

  _currentTransform._relativeRotRad = b2Rot_GetAngle(b2MulRot(
      b2MakeRot(_currentTransform._relativeRotRad), b2MakeRot(B2_PI / 100)));
  // _currentTransform._relativePos=
  // b2Add(_currentTransform._relativePos,{0.1,0.1});

  RenderUtils::renderTexture(texture.get(), region, 10, 10, _currentTransform,
                             renderContext);
}
