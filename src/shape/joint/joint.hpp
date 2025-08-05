#pragma once

#include "box2d/box2d.h"
#include "box2d/types.h"
#include "entt/entity/entity.hpp"
#include "entt/entt.hpp"
struct JointConfig {};
class Joint {
public:
  b2JointId getJointId();
  b2JointType getType();
  entt::entity getEntity();
  virtual ~Joint();

protected:
  Joint();
  Joint(Joint &other) = delete;
  Joint(Joint &&other) = delete;
  Joint &operator=(Joint &other) = delete;
  Joint &operator=(Joint &&other) = delete;

  b2JointId jointId;
  entt::entity entity = entt::null;

  friend class PhysicsFactory;
};