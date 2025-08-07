#pragma once

#include "box2d/box2d.h"
#include "box2d/id.h"
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

  // Removes a joint from the registry if registered. Does not automatically
  // delete the shape as shared pointers are responsible for deletion
  void remove();

protected:
  Joint() = delete;
  Joint(entt::registry &registry);
  Joint(Joint &other) = delete;
  Joint(Joint &&other) = delete;
  Joint &operator=(Joint &other) = delete;
  Joint &operator=(Joint &&other) = delete;

  b2JointId jointId = b2_nullJointId;
  entt::entity entity = entt::null;
  entt::registry &registry;

  friend class PhysicsFactory;
};