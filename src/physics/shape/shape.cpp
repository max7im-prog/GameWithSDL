#include "shape.hpp"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include "registryComposite.hpp"
#include "renderComponents.hpp"

b2BodyId Shape::getBodyId() { return bodyId; }

b2ShapeId Shape::getShapeId() { return shapeId; };

b2ShapeType Shape::getType() { return b2Shape_GetType(shapeId); }

Shape::Shape(entt::registry &registry) : RegistryComposite(registry) {}

Shape::~Shape() {
  if (b2Body_IsValid(bodyId)) {
    b2DestroyBody(bodyId);
  }
  bodyId = b2_nullBodyId;
  shapeId = b2_nullShapeId;
}

float Shape::getMass() { return b2Body_GetMass(bodyId); }

float Shape::getRotationalInertia() {
  return b2Body_GetRotationalInertia(bodyId);
}

b2Rot Shape::getRotation() { return b2Body_GetRotation(bodyId); }

void Shape::applyTorque(float torque) {
  b2Body_ApplyTorque(bodyId, torque, true);
}

b2Vec2 Shape::getLinearVelocity() { return b2Body_GetLinearVelocity(bodyId); }

b2Vec2 Shape::getLocalPoint(b2Vec2 worldPoint) {
  return b2Body_GetLocalPoint(bodyId, worldPoint);
}

b2Vec2 Shape::getWorldPoint(b2Vec2 localPoint) const {
  return b2Body_GetWorldPoint(bodyId, localPoint);
}

b2Vec2 Shape::getWorldPos() { return b2Body_GetWorldPoint(bodyId, {0, 0}); }

void Shape::performRotation(b2Rot rot) {
  auto curRot = get3dRot();
  if (b2Rot_GetAngle(rot) == b2Rot_GetAngle(curRot)) {
    return;
  }

  registry.emplace_or_replace<RenderRequiresUpdateTag>(getEntity());

  // TODO: complete
}
