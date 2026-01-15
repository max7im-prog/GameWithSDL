#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

class WeldJoint : public Joint, public VisitableImpl<WeldJoint> {
public:
  struct Config : public Joint::Config {
    void defaultConfig() override;
    b2WeldJointDef jointDef;
  };
  ~WeldJoint() = default;

protected:
  WeldJoint() = delete;
  WeldJoint(entt::registry &registry, const World &world,
            const WeldJoint::Config &config);

  friend class JointFactory;
};
