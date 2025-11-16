#pragma once
#include "SDL3/SDL_render.h"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>


class TextureManager {
public:
  std::optional<SDL_Texture*> getTexture(const std::string& filename);
protected:
private:
  std::unordered_map<std::string,
                     std::unique_ptr<SDL_Texture, decltype([](SDL_Texture *t) {
                                       SDL_DestroyTexture(t);
                                     })>>
      _textures;
};