#include "mouseJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

void MouseJoint::Config::defaultConfig() {
  jointDef = b2DefaultMouseJointDef();
}

MouseJoint::MouseJoint(entt::registry &registry, const World &world,
                       const MouseJoint::Config &config)
    : Joint(registry) {
  jointId = b2CreateMouseJoint(world.getWorldId(), &(config.jointDef));
}
