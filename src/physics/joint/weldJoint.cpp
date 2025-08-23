#include "weldJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

WeldJointConfig WeldJointConfig::defaultConfig() {
  WeldJointConfig ret;
  ret.jointDef = b2DefaultWeldJointDef();
  return ret;
}

WeldJoint::WeldJoint(entt::registry &registry, const World &world,
                     const WeldJointConfig &config)
    : Joint(registry) {
  jointId = b2CreateWeldJoint(world.getWorldId(), &(config.jointDef));
}