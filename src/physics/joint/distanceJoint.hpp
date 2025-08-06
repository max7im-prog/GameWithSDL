#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

struct DistanceJointConfig : public JointConfig {
  static DistanceJointConfig defaultConfig();
  b2DistanceJointDef jointDef;
};

class DistanceJoint : public Joint {
public:
  ~DistanceJoint() = default;

protected:
  DistanceJoint() = delete;
  DistanceJoint(entt::registry &registry,const World &world, const DistanceJointConfig &config);

  friend class PhysicsFactory;
};