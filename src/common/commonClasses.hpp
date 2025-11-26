#pragma once

#include "box2d/math_functions.h"
namespace Common {

/**
 * @brief Defines an object's transform in the world
 *
 */
struct Transform {
  b2Vec2 _originPos{0, 0};
  b2Vec2 _relativePos{0, 0};
  b2Rot _rootRot{b2MakeRot(0)};
  b2Rot _relativeRot{b2MakeRot(0)};
  float _scaleX{1.0f};
  float _scaleY{1.0f};
  bool _flipX{false};
  bool _flipY{false};
};


}; // namespace Common