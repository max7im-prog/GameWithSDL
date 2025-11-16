#pragma once
#include "SDL3/SDL_render.h"
#include "renderContext.hpp"
#include <memory>
#include <string>
#include <unordered_map>

class TextureManager {
public:
  std::shared_ptr<SDL_Texture> getTexture(const std::string &filename);
  TextureManager(const RenderContext &renderContext);

protected:
private:
  const RenderContext &_renderContext;
  std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> _textures;
};