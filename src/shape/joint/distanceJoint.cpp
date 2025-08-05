#include "distanceJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

DistanceJointConfig DistanceJointConfig::defaultConfig() {
  DistanceJointConfig ret;
  ret.jointDef = b2DefaultDistanceJointDef();
  return ret;
}

DistanceJoint::DistanceJoint(const World &world,
                             const DistanceJointConfig &config) {
  jointId = b2CreateDistanceJoint(world.getWorldId(), &(config.jointDef));
}