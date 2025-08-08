#include "bodyPart.hpp"

BodyPart::~BodyPart() {
  remove();

  for (auto j : joints) {
    j->remove();
  }

  for (auto sh : shapes) {
    sh->remove();
  }
}

entt::entity BodyPart::getEntity() { return entity; }

BodyPart::BodyPart(entt::registry &registry, const std::shared_ptr<World> world)
    : world(world), registry(registry) {}

void BodyPart::remove() {
  if (registry.valid(entity)) {
    registry.destroy(entity);
  }
  entity = entt::null;
}

void BodyPart::registerShape(std::shared_ptr<Shape> shape) {
  shapes.insert(shape);
}

void BodyPart::registerJoint(std::shared_ptr<Joint> joint) {
  joints.insert(joint);
}