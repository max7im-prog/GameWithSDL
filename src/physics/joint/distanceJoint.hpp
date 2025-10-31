#pragma once

#include "box2d/types.h"
#include "joint.hpp"
#include "world.hpp"

struct DistanceJointConfig : public JointConfig {
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;
  b2DistanceJointDef jointDef;
};

class DistanceJoint : public Joint , public Visitable<DistanceJoint>{
public:
  using Config = DistanceJointConfig;
  ~DistanceJoint() = default;

protected:
  DistanceJoint() = delete;
  DistanceJoint(entt::registry &registry,const World &world, const DistanceJointConfig &config);

  friend class JointFactory;
};