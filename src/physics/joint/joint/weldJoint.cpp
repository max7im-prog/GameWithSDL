#include "weldJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

void WeldJoint::Config::defaultConfig() { jointDef = b2DefaultWeldJointDef(); }

WeldJoint::WeldJoint(entt::registry &registry, const World &world,
                     const WeldJoint::Config &config)
    : Joint(registry) {
  jointId = b2CreateWeldJoint(world.getWorldId(), &(config.jointDef));
}
