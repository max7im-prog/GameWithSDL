#include "bodyPart.hpp"
#include "registryObject.hpp"

BodyPart::~BodyPart() {
  for (auto j : joints) {
    j->remove();
  }

  for (auto sh : shapes) {
    sh->remove();
  }
}

BodyPart::BodyPart(entt::registry &registry, const std::shared_ptr<World> world)
    : RegistryObject(registry), world(world) {}

void BodyPart::registerShape(std::shared_ptr<Shape> shape) {
  shapes.push_back(shape);
}

void BodyPart::registerJoint(std::shared_ptr<Joint> joint) {
  joints.push_back(joint);
}