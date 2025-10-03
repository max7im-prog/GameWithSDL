#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

struct MouseJointConfig : public JointConfig {
  static MouseJointConfig defaultConfig();
  b2MouseJointDef jointDef;
};

class MouseJoint : public Joint , public Visitable<MouseJoint>{
public:
  using Config = MouseJointConfig;
  ~MouseJoint() = default;

protected:
  MouseJoint() = delete;
  MouseJoint(entt::registry &registry,const World &world, const MouseJointConfig &config);

  friend class JointFactory;
};