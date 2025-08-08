#pragma once

#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/types.h"
#include "entt/entity/entity.hpp"
#include "entt/entt.hpp"
#include "registryObject.hpp"

struct JointConfig {};

class Joint : public RegistryObject {
public:
  b2JointId getJointId();
  b2JointType getType();
  virtual ~Joint();

protected:
  Joint() = delete;
  Joint(entt::registry &registry);
  Joint(Joint &other) = delete;
  Joint(Joint &&other) = delete;
  Joint &operator=(Joint &other) = delete;
  Joint &operator=(Joint &&other) = delete;

  b2JointId jointId = b2_nullJointId;

  friend class PhysicsFactory;
};