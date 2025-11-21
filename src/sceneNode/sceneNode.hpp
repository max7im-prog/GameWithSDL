#pragma once

#include "box2d/math_functions.h"
#include "registryComposite.hpp"
#include "visitor.hpp"
class SceneNode : public RegistryComposite, public virtual IVisitable {
public:
  ~SceneNode() = 0;
  virtual b2Vec2 getWorldPos() = 0;

protected:
  SceneNode(entt::registry &registry);
};