#pragma once

#include "joint.hpp"
#include "objectConfig.hpp"
#include "registryComposite.hpp"
#include "shape.hpp"
class Structure : public RegistryComposite {
public:
  class Config : public ObjectConfig {};
  virtual ~Structure() = 0;

  const std::unordered_map<std::string, std::weak_ptr<Shape>> &
  getShapes() const;
  const std::unordered_map<std::string, std::weak_ptr<Joint>> &
  getJoints() const;

  void registerShape(std::weak_ptr<Shape> body, const std::string &name);
  void registerJoint(std::weak_ptr<Joint> connection, const std::string &name);

protected:
  std::unordered_map<std::string, std::weak_ptr<Shape>> _shapes;
  std::unordered_map<std::string, std::weak_ptr<Joint>> _joints;
  Structure(entt::registry &registry);
};
