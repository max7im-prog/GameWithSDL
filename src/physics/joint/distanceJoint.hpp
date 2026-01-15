#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

class DistanceJoint : public Joint, public VisitableImpl<DistanceJoint> {
public:
  struct Config : public Joint::Config {
    void defaultConfig() override;
    b2DistanceJointDef jointDef;
  };
  ~DistanceJoint() = default;

protected:
  DistanceJoint() = delete;
  DistanceJoint(entt::registry &registry, const World &world,
                const DistanceJoint::Config &config);

  friend class JointFactory;
};
