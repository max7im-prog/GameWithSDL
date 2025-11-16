#include "textureManager.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include <SDL3_image/SDL_image.h>
#include <memory>

TextureManager::TextureManager(const RenderContext &renderContext)
    : _renderContext(renderContext) {}

std::shared_ptr<SDL_Texture>
TextureManager::getTexture(const std::string &filename) {
  if (_textures.find(filename) == _textures.end()) {
    // Try to load texture
    std::shared_ptr<SDL_Surface> surface{IMG_Load(filename.c_str()),
                                         SDL_DestroySurface};
    if (!surface) {
      // TODO: log error
      return nullptr;
    }

    std::shared_ptr<SDL_Texture> texture{
        SDL_CreateTextureFromSurface(_renderContext.getSDLRenderer(),
                                     surface.get()),
        SDL_DestroyTexture};
    if (!texture) {
      // TODO: log error
      return nullptr;
    }

    _textures.emplace(filename, texture);
    return texture;
  }

  return _textures[filename];
}