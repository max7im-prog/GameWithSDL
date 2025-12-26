#include "body.hpp"
#include "registryComposite.hpp"

Body::~Body() {}

Body::Body(entt::registry &registry, const std::shared_ptr<World> world)
    : RegistryComposite(registry), world(world) {}

void Body::registerJoint(std::weak_ptr<Joint> joint, const std::string &name) {
  if (_joints.find(name) != _joints.end()) {
    throw std::runtime_error("Joint with name " + name +
                             " registered more than once");
  }
  _joints[name] = joint;
  registerChild(joint);
}

void Body::registerShape(std::weak_ptr<Shape> shape, const std::string &name) {
  if (_shapes.find(name) != _shapes.end()) {
    throw std::runtime_error("Shape with name " + name +
                             " registered more than once");
  }
  _shapes[name] = shape;
  registerChild(shape);
}

const std::unordered_map<std::string, std::weak_ptr<Shape>> &
Body::getShapes() const {
  return _shapes;
}

const std::unordered_map<std::string, std::weak_ptr<Joint>> &
Body::getJoints() const {
  return _joints;
}

void Body::performRotation(b2Rot rot) {
  // TODO: complete
}
