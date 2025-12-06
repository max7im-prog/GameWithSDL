#include "commonClasses.hpp"
Common::Transform
Common::Transform::composeTransform(const Common::Transform &a,
                                    const Common::Transform &b) {
  Common::Transform ret;
  ret._flipX = (a._flipX != b._flipX);
  ret._flipY = (a._flipY != b._flipY);
  ret._originPos = b2Add(a._originPos, b._originPos);
  ret._relativePos = b2Add(a._relativePos, b._relativePos);
  ret._rootRot = b2MulRot(a._rootRot, b._rootRot);
  ret._relativeRot = b2MulRot(a._relativeRot, b._relativeRot);
  ret._scaleX = a._scaleX * b._scaleX;
  ret._scaleY = a._scaleY * b._scaleY;

  return ret;
}
