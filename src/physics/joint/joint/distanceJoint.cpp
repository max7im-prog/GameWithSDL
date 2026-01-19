#include "distanceJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

void DistanceJoint::Config::defaultConfig() {
  jointDef = b2DefaultDistanceJointDef();
}

DistanceJoint::DistanceJoint(entt::registry &registry, const World &world,
                             const DistanceJoint::Config &config)
    : Joint(registry) {
  jointId = b2CreateDistanceJoint(world.getWorldId(), &(config.jointDef));
}
