#pragma once

#include "joint.hpp"
#include "objectConfig.hpp"
#include "registryComposite.hpp"
#include "shape.hpp"
class Structure : public RegistryComposite {
public:
  using ShapeId = std::string;
  using JointId = std::string;
  class Config : public ObjectConfig {};
  virtual ~Structure() = 0;

  const std::unordered_map<std::string, std::weak_ptr<Shape>> &
  getShapes() const;
  const std::unordered_map<std::string, std::weak_ptr<Joint>> &
  getJoints() const;

  void registerShape(std::weak_ptr<Shape> body, std::string_view name);
  void registerJoint(std::weak_ptr<Joint> connection, std::string_view name);

protected:
  std::unordered_map<std::string, std::weak_ptr<Shape>> _shapes;
  std::unordered_map<std::string, std::weak_ptr<Joint>> _joints;
  Structure(entt::registry &registry);
};
