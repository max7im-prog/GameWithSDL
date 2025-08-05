#pragma once
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
  entt::entity getEntity();
  b2ShapeType getType();

protected:
  Shape();
  Shape(Shape &other) = delete;
  Shape(Shape &&other) = delete;
  Shape &operator=(Shape &other) = delete;
  Shape &operator=(Shape &&other) = delete;
  virtual ~Shape() = default;

  b2BodyId bodyId;
  b2ShapeId shapeId;
  entt::entity entity;

  friend class PhysicsFactory;
};
