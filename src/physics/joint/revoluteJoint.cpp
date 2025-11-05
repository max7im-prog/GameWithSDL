#include "revoluteJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

void RevoluteJointConfig::defaultConfig() {
  jointDef = b2DefaultRevoluteJointDef();
}

RevoluteJoint::RevoluteJoint(entt::registry &registry, const World &world,
                             const RevoluteJointConfig &config)
    : Joint(registry) {
  jointId = b2CreateRevoluteJoint(world.getWorldId(), &(config.jointDef));
}

void RevoluteJoint::setAngleLimits(float lower, float upper) {
  b2RevoluteJoint_SetLimits(jointId, lower, upper);
}