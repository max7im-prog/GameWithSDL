#pragma once

#include "box2d/types.h"
#include "entt/entt.hpp"
#include "physicsPrimitive.hpp"
#include "visitor.hpp"

class Joint : public PhysicsPrimitive, public virtual IVisitable {
public:
  struct Config : public PhysicsPrimitive::Config {};
  b2JointId getJointId();
  b2JointType getType();
  virtual ~Joint();

  void update([[maybe_unused]] float dt) override {};

protected:
  Joint() = delete;
  Joint(entt::registry &registry);

  b2JointId jointId = b2_nullJointId;

  friend class JointFactory;
};
