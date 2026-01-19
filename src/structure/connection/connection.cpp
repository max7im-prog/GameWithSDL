#include "connection.hpp"
#include "registryComposite.hpp"

Connection::Connection(entt::registry &registry,
                       const std::shared_ptr<World> world)
    : RegistryComposite(registry), world(world) {}

Connection::~Connection() {}

void Connection::registerJoint(std::weak_ptr<Joint> joint, const std::string &name) {
  if (_joints.find(name) != _joints.end()) {
    throw std::runtime_error("Joint with name " + name +
                             " registered more than once");
  }
  _joints[name] = joint;
  registerChild(joint);
}

void Connection::registerShape(std::weak_ptr<Shape> shape, const std::string &name) {
  if (_shapes.find(name) != _shapes.end()) {
    throw std::runtime_error("Shape with name " + name +
                             " registered more than once");
  }
  _shapes[name] = shape;
  registerChild(shape);
}

const std::unordered_map<std::string, std::weak_ptr<Shape>> &
Connection::getShapes() const {
  return _shapes;
}

const std::unordered_map<std::string, std::weak_ptr<Joint>> &
Connection::getJoints() const {
  return _joints;
}