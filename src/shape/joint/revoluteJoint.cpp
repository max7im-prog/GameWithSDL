#include "revoluteJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

RevoluteJointConfig RevoluteJointConfig::defaultConfig() {
  RevoluteJointConfig ret;
  ret.jointDef = b2DefaultRevoluteJointDef();
  return ret;
}

RevoluteJoint::RevoluteJoint(const World &world,
                             const RevoluteJointConfig &config) {
  jointId = b2CreateRevoluteJoint(world.getWorldId(), &(config.jointDef));
}