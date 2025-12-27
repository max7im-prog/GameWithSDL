#include "body.hpp"
#include "box2d/math_functions.h"
#include "registryComposite.hpp"
#include "renderComponents.hpp"

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
  auto curRot = get3dRot();
  if (b2Rot_GetAngle(rot) == b2Rot_GetAngle(curRot)) {
    return;
  }

  _registry.emplace_or_replace<RenderRequiresUpdateTag>(getEntity());
  auto rotIncr = b2InvMulRot(curRot, rot);
  for (auto &[name, shape] : getShapes()) {
    if (auto lock = shape.lock()) {
      b2Rot shapeNewRot = b2MulRot(rotIncr, lock->get3dRot());
      lock->set3dRot(shapeNewRot);
    }
  }
}
