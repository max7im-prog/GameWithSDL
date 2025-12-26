#pragma once

#include "box2d/math_functions.h"

/**
 *
 * @brief an interface for pseudo-3d elements
 *
 *    y
 *    |
 *    |    __
 *    |   |  |
 *    |   |__|
 *    |
 *    |_______________x
 *   .
 *  .
 * z
 *
 * Rotates an object around a Y axis in direction from Z to X
 */
class Pseudo3d {
public:
  virtual ~Pseudo3d() = 0;
  void set3dRot(b2Rot rot);
  b2Rot get3dRot();

protected:
  /**
   *
   * @brief a fucntion that will be overriden by derived classes to perform
   * actions during the rotation process
   *
   * @param rot new rotation value
   */
  virtual void performRotation(b2Rot rot) = 0;

private:
  b2Rot _3dRot{b2MakeRot(0)};
};
