#include "shape.hpp"
#include "box2d/box2d.h"

b2BodyId Shape::getBodyId() { return bodyId; }

b2ShapeId Shape::getShapeId(){return shapeId;};

entt::entity Shape::getEntity() { return entity; }

b2ShapeType Shape::getType() { return b2Shape_GetType(shapeId); }

Shape::Shape() {}

Shape::~Shape() {
  if (b2Body_IsValid(bodyId)) {
    b2DestroyBody(bodyId);
  }
}
