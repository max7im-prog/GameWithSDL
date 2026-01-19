#pragma once
#include "box2d/id.h"
#include "objectConfig.hpp"
#include "physicsPrimitive.hpp"
#include "pseudo3d.hpp"
#include "visitor.hpp"
#include "world.hpp"
#include <box2d/box2d.h>
#include <box2d/types.h>
#include <entt/entt.hpp>
#include <optional>

class Shape : public PhysicsPrimitive,
              public virtual IVisitable,
              public Pseudo3d {
public:
  struct Config : public PhysicsPrimitive::Config {
    b2BodyDef bodyDef;
    b2ShapeDef shapeDef;
  };
  b2BodyId getBodyId();
  b2ShapeId getShapeId();
  b2ShapeType getType();
  float getMass();
  float getRotationalInertia();
  b2Rot getRotation();
  b2Vec2 getLinearVelocity();
  b2Vec2 getLocalPoint(b2Vec2 worldPoint);
  b2Vec2 getWorldPoint(b2Vec2 localPoint) const;
  b2Vec2 getWorldPos();

  void applyTorque(float torque);

  virtual ~Shape();
  void update([[maybe_unused]] float dt) override {};

protected:
  Shape() = delete;
  Shape(entt::registry &registry);

  b2BodyId bodyId = b2_nullBodyId;
  b2ShapeId shapeId = b2_nullShapeId;

  virtual void performRotation(b2Rot rot) override;

  friend class ShapeFactory;
};
