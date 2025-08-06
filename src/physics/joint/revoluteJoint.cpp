#include "revoluteJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

RevoluteJointConfig RevoluteJointConfig::defaultConfig() {
  RevoluteJointConfig ret;
  ret.jointDef = b2DefaultRevoluteJointDef();
  return ret;
}

RevoluteJoint::RevoluteJoint(entt::registry &registry, const World &world,
                             const RevoluteJointConfig &config)
    : Joint(registry) {
  jointId = b2CreateRevoluteJoint(world.getWorldId(), &(config.jointDef));
}