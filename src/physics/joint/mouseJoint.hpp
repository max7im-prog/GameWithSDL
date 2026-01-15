#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

class MouseJoint : public Joint, public VisitableImpl<MouseJoint> {
public:
  struct Config : public Joint::Config {
    void defaultConfig() override;
    b2MouseJointDef jointDef;
  };
  ~MouseJoint() = default;

protected:
  MouseJoint() = delete;
  MouseJoint(entt::registry &registry, const World &world,
             const MouseJoint::Config &config);

  friend class JointFactory;
};
