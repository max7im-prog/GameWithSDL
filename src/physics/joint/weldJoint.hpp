#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

struct WeldJointConfig : public JointConfig {
  void defaultConfig() override;
  b2WeldJointDef jointDef;
};

class WeldJoint : public Joint , public Visitable<WeldJoint>{
public:
  using Config = WeldJointConfig;
  ~WeldJoint() = default;

protected:
  WeldJoint() = delete;
  WeldJoint(entt::registry &registry, const World &world,
            const WeldJointConfig &config);

  friend class JointFactory;
};