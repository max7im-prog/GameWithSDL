
#include "prismaticJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

void PrismaticJointConfig::defaultConfig() {
  jointDef = b2DefaultPrismaticJointDef();
}


PrismaticJoint::PrismaticJoint(entt::registry &registry, const World &world,
                               const PrismaticJointConfig &config)
    : Joint(registry) {
  jointId = b2CreatePrismaticJoint(world.getWorldId(), &(config.jointDef));
}

float PrismaticJoint::getTargetTranslation() {
  return b2PrismaticJoint_GetTargetTranslation(jointId);
}

void PrismaticJoint::setTargetTranslation(float translation) {
  b2PrismaticJoint_SetTargetTranslation(jointId, translation);
}