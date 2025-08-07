#include "mouseJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

MouseJointConfig MouseJointConfig::defaultConfig() {
  MouseJointConfig ret;
  ret.jointDef = b2DefaultMouseJointDef();
  return ret;
}

MouseJoint::MouseJoint(entt::registry &registry, const World &world,
                             const MouseJointConfig &config)
    : Joint(registry) {
  jointId = b2CreateMouseJoint(world.getWorldId(), &(config.jointDef));
}