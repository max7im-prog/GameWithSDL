#pragma once

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include <memory>
#include <string>
#include <vector>

struct TextureComponent {
  std::shared_ptr<SDL_Texture> _texture;
  int _numTextures;
  SDL_FRect _initialRect;
  SDL_FRect _currentRect;
  SDL_FPoint _offsetPerTexture;
  struct {
    float _w;
    float _h;
  } _worldSize;
};

struct RenderSequenceComponent {
  using ChildId = std::string;
  std::vector<ChildId> _renderSequence;
};
