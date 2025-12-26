#include "pseudo3d.hpp"

Pseudo3d::~Pseudo3d() = default;
void Pseudo3d::set3dRot(b2Rot rot) {
  performRotation(rot);
  _3dRot = rot;
}
b2Rot Pseudo3d::get3dRot() { return _3dRot; }
