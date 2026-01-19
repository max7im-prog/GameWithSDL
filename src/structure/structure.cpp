
#include "structure.hpp"

Structure::~Structure() = default;

Structure::Structure(entt::registry &registry) : RegistryComposite(registry) {}

void Structure::registerJoint(std::weak_ptr<Joint> joint,
                              const std::string &name) {
  if (_joints.find(name) != _joints.end()) {
    throw std::runtime_error("Joint with name " + name +
                             " registered more than once");
  }
  _joints[name] = joint;
  registerChild(joint);
}

void Structure::registerShape(std::weak_ptr<Shape> shape,
                              const std::string &name) {
  if (_shapes.find(name) != _shapes.end()) {
    throw std::runtime_error("Shape with name " + name +
                             " registered more than once");
  }
  _shapes[name] = shape;
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
