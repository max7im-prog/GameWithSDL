#pragma once

#include "SDL3/SDL_render.h"
#include <memory>
#include <string>
#include <vector>

struct TextureComponent {
  using TextureId = std::string;
  std::vector<std::shared_ptr<SDL_Texture>> _textures;
};

struct RenderSequenceComponent {
  using ChildId = std::string;
  std::vector<ChildId> _renderSequence;
};