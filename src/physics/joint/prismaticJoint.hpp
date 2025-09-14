
#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

struct PrismaticJointConfig : public JointConfig {
  static PrismaticJointConfig defaultConfig();
  b2PrismaticJointDef jointDef;
};

class PrismaticJoint : public Joint {
public:
  using Config = PrismaticJointConfig;
  ~PrismaticJoint() = default;
  float getTargetTranslation();
  void setTargetTranslation(float translation);

protected:
  PrismaticJoint() = delete;
  PrismaticJoint(entt::registry &registry, const World &world,
                const PrismaticJointConfig &config);

  friend class JointFactory;
};