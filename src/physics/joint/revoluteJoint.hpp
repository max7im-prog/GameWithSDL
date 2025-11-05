#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

struct RevoluteJointConfig : public JointConfig {
  void defaultConfig() override;
  b2RevoluteJointDef jointDef;
};

class RevoluteJoint : public Joint , public Visitable<RevoluteJoint>{
public:
  using Config = RevoluteJointConfig;
  ~RevoluteJoint() = default;
  void setAngleLimits(float lower, float upper);

protected:
  RevoluteJoint() = delete;
  RevoluteJoint(entt::registry &registry, const World &world,
                const RevoluteJointConfig &config);

  friend class JointFactory;
};