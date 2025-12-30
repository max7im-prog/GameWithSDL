#include "pseudo3d.hpp"
#include "box2d/math_functions.h"

Pseudo3d::~Pseudo3d() = default;

void Pseudo3d::set3dRot(b2Rot rot) {
  performRotation(rot);
  _3dAngle = b2Rot_GetAngle(rot);
}

b2Rot Pseudo3d::get3dRot() const { return b2MakeRot(_3dAngle); }
