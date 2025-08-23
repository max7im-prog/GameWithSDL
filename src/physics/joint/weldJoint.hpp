#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

struct WeldJointConfig : public JointConfig {
  static WeldJointConfig defaultConfig();
  b2WeldJointDef jointDef;
};

class WeldJoint : public Joint {
public:
  ~WeldJoint() = default;

protected:
  WeldJoint() = delete;
  WeldJoint(entt::registry &registry,const World &world, const WeldJointConfig &config);

  friend class PhysicsFactory;
};