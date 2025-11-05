#include "weldJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

void WeldJointConfig::defaultConfig() {
  jointDef = b2DefaultWeldJointDef();
}

WeldJoint::WeldJoint(entt::registry &registry, const World &world,
                     const WeldJointConfig &config)
    : Joint(registry) {
  jointId = b2CreateWeldJoint(world.getWorldId(), &(config.jointDef));
}