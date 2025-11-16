#include "textureManager.hpp"
#include "SDL3/SDL.h"
#include <memory>
#include <SDL3_image/SDL_image.h>

std::optional<SDL_Texture *>
TextureManager::getTexture(const std::string &filename) {
  using SurfacePtr =
      std::unique_ptr<SDL_Surface,
                      decltype([](SDL_Surface *s) { SDL_DestroySurface(s); })>;
  if (_textures.find(filename) == _textures.end()) {
    // Try to load texture
  
    // SurfacePtr surface = SurfacePtr(SDL_L);
  }
}