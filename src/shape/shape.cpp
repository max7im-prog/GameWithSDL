#include "shape.hpp"
#include "box2d/box2d.h"

b2BodyId Shape::getBodyId() { return bodyId; }

entt::entity Shape::getEntity() { return entity; }

b2ShapeType Shape::getType() { return b2Shape_GetType(shapeId); }

Shape::Shape() {}
