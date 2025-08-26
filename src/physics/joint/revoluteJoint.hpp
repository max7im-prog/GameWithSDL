#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

struct RevoluteJointConfig : public JointConfig {
  static RevoluteJointConfig defaultConfig();
  b2RevoluteJointDef jointDef;
};

class RevoluteJoint : public Joint {
public:
  using Config = RevoluteJointConfig;
  ~RevoluteJoint() = default;

protected:
  RevoluteJoint() = delete;
  RevoluteJoint(entt::registry &registry, const World &world,
                const RevoluteJointConfig &config);

  friend class JointFactory;
};