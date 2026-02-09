
#include "structure.hpp"

Structure::~Structure() = default;

Structure::Structure(entt::registry &registry) : RegistryComposite(registry) {}

void Structure::registerJoint(std::weak_ptr<Joint> joint,
                              std::string_view name) {

  std::string nameStr = static_cast<std::string>(name);

  if (_joints.find(nameStr) != _joints.end()) {
    throw std::runtime_error("Joint with name " + nameStr +
                             " registered more than once");
  }
  _joints[nameStr] = joint;
  registerChild(joint);
}

void Structure::registerShape(std::weak_ptr<Shape> shape,
                              std::string_view name) {
  std::string nameStr = static_cast<std::string>(name);
  if (_shapes.find(nameStr) != _shapes.end()) {
    throw std::runtime_error("Shape with name " + nameStr +
                             " registered more than once");
  }
  _shapes[nameStr] = shape;
  registerChild(shape);
}

const std::unordered_map<std::string, std::weak_ptr<Shape>> &
Structure::getShapes() const {
  return _shapes;
}

const std::unordered_map<std::string, std::weak_ptr<Joint>> &
Structure::getJoints() const {
  return _joints;
}
