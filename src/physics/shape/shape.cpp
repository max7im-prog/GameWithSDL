#include "shape.hpp"
#include "box2d/box2d.h"
#include "registryObject.hpp"

b2BodyId Shape::getBodyId() { return bodyId; }

b2ShapeId Shape::getShapeId() { return shapeId; };

b2ShapeType Shape::getType() { return b2Shape_GetType(shapeId); }

Shape::Shape(entt::registry &registry) : RegistryObject(registry) {}

Shape::~Shape() {
  if (b2Body_IsValid(bodyId)) {
    b2DestroyBody(bodyId);
  }
}

float Shape::getMass() { return b2Body_GetMass(bodyId); }

float Shape::getRotationalInertia() {
  return b2Body_GetRotationalInertia(bodyId);
}

b2Rot Shape::getRotation() { return b2Body_GetRotation(bodyId); }

void Shape::applyTorque(float torque) {
  b2Body_ApplyTorque(bodyId, torque, true);
}
