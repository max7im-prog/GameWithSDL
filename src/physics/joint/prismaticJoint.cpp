
#include "prismaticJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

PrismaticJointConfig PrismaticJointConfig::defaultConfig() {
  PrismaticJointConfig ret;
  ret.jointDef = b2DefaultPrismaticJointDef();
  return ret;
}

PrismaticJoint::PrismaticJoint(entt::registry &registry, const World &world,
                             const PrismaticJointConfig &config)
    : Joint(registry) {
  jointId = b2CreatePrismaticJoint(world.getWorldId(), &(config.jointDef));
}