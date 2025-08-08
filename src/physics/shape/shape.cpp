#include "shape.hpp"
#include "box2d/box2d.h"

b2BodyId Shape::getBodyId() { return bodyId; }

b2ShapeId Shape::getShapeId() { return shapeId; };

entt::entity Shape::getEntity() { return entity; }

b2ShapeType Shape::getType() { return b2Shape_GetType(shapeId); }

Shape::Shape(entt::registry &registry) : registry(registry) {}

Shape::~Shape() {
  if (b2Body_IsValid(bodyId)) {
    b2DestroyBody(bodyId);
  }
  remove();
}

void Shape::remove() {
  if (registry.valid(entity)) {
    registry.destroy(entity);
  }
  entity = entt::null;
}

float Shape::getMass(){
  return b2Body_GetMass(bodyId);
}