#pragma once
#include "box2d/id.h"
#include "objectConfig.hpp"
#include "pseudo3d.hpp"
#include "registryComposite.hpp"
#include "visitor.hpp"
#include "world.hpp"
#include <box2d/box2d.h>
#include <box2d/types.h>
#include <entt/entt.hpp>
#include <optional>

struct ShapeConfig : public ObjectConfig {
  b2BodyDef bodyDef;
  b2ShapeDef shapeDef;
};

class Shape : public RegistryComposite,
              public virtual IVisitable,
              public Pseudo3d {
public:
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
  Shape(Shape &other) = delete;
  Shape(Shape &&other) = delete;
  Shape &operator=(Shape &other) = delete;
  Shape &operator=(Shape &&other) = delete;

  b2BodyId bodyId = b2_nullBodyId;
  b2ShapeId shapeId = b2_nullShapeId;

  virtual void performRotation(b2Rot rot) override;

  friend class ShapeFactory;
};
