#pragma once

#include <string>
#include <vector>

struct TextureComponent {
  using TextureId = std::string;
  TextureId _textureId;
};

struct RenderSequence {
  using ChildId = std::string;
  std::vector<ChildId> _renderSequence;
};