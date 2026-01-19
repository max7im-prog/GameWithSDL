#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

class RevoluteJoint : public Joint, public VisitableImpl<RevoluteJoint> {
public:
  struct Config : public Joint::Config {
    void defaultConfig() override;
    b2RevoluteJointDef jointDef;
  };
  ~RevoluteJoint() = default;
  void setAngleLimits(float lower, float upper);

protected:
  RevoluteJoint() = delete;
  RevoluteJoint(entt::registry &registry, const World &world,
                const RevoluteJoint::Config &config);

  friend class JointFactory;
};
