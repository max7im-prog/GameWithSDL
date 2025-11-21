#pragma once

#include "box2d/math_functions.h"
#include "registryComposite.hpp"
#include "visitor.hpp"
class SceneNode : public RegistryComposite, public virtual IVisitable {
public:
  ~SceneNode() = 0;
  virtual b2Vec2 getWorldPos() = 0;
  const std::unordered_map<std::string, std::weak_ptr<Body>> &getBodies() const;
  const std::unordered_map<std::string, std::weak_ptr<Connection>> &getConnections() const;

protected:
  SceneNode(entt::registry &registry);
  std::unordered_map<std::string, std::weak_ptr<Body>> _bodies;
  std::unordered_map<std::string, std::weak_ptr<Connection>> _connections;
};