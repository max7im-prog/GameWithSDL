
#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

class PrismaticJoint : public Joint, public VisitableImpl<PrismaticJoint> {
public:
  struct Config : public Joint::Config {
    void defaultConfig() override;
    b2PrismaticJointDef jointDef;
  };
  ~PrismaticJoint() = default;
  float getTargetTranslation();
  void setTargetTranslation(float translation);

protected:
  PrismaticJoint() = delete;
  PrismaticJoint(entt::registry &registry, const World &world,
                 const PrismaticJoint::Config &config);

  friend class JointFactory;
};
