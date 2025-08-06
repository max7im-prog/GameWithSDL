#pragma once
#include "box2d/id.h"
#include "world.hpp"
#include <box2d/box2d.h>
#include <box2d/types.h>
#include <entt/entt.hpp>
#include <optional>

struct ShapeConfig {
  b2BodyDef bodyDef;
  b2ShapeDef shapeDef;
};

class Shape {
public:
  b2BodyId getBodyId();
  b2ShapeId getShapeId();
  entt::entity getEntity();
  b2ShapeType getType();

  virtual ~Shape();

protected:
  Shape() = delete;
  Shape(entt::registry &registry);
  Shape(Shape &other) = delete;
  Shape(Shape &&other) = delete;
  Shape &operator=(Shape &other) = delete;
  Shape &operator=(Shape &&other) = delete;

  b2BodyId bodyId = b2_nullBodyId;
  b2ShapeId shapeId = b2_nullShapeId;
  entt::entity entity = entt::null;
  entt::registry &registry;

  friend class PhysicsFactory;
};
