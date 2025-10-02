#pragma once

#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/types.h"
#include "entt/entity/entity.hpp"
#include "entt/entt.hpp"
#include "registryComposite.hpp"
#include "visitor.hpp"

struct JointConfig {};

class Joint : public RegistryComposite , public SceneNode{
public:
  b2JointId getJointId();
  b2JointType getType();
  virtual ~Joint();

  void update([[maybe_unused]] float dt) override {};

  virtual void accept(Visitor &v) override;
protected:
  Joint() = delete;
  Joint(entt::registry &registry);
  Joint(Joint &other) = delete;
  Joint(Joint &&other) = delete;
  Joint &operator=(Joint &other) = delete;
  Joint &operator=(Joint &&other) = delete;

  b2JointId jointId = b2_nullJointId;

  friend class JointFactory;
};