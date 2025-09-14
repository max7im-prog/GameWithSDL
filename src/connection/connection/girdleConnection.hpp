#pragma once

#include "box2d/box2d.h"
#include "box2d/collision.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include "capsule.hpp"
#include "circle.hpp"
#include "connection.hpp"
#include "distanceJoint.hpp"
#include "emptyShape.hpp"
#include "jointFactory.hpp"
#include "revoluteJoint.hpp"
#include "shapeFactory.hpp"
#include "weldJoint.hpp"
#include "world.hpp"
#include <memory>

struct GirdleConnectionConfig : public ConnectionConfig {
  static GirdleConnectionConfig defaultConfig();
  b2Filter filter;

  struct {
    CircleConfig leftTemplate;
    CircleConfig rightTemplate;
    CircleConfig centerTemplate;
    PrismaticJointConfig prismTemplate;
  } configs;

  struct {
    std::shared_ptr<Shape> shape = nullptr;
    b2Vec2 localPoint = {0, 0};
    RevoluteJointConfig attachTemplate;
  } leftAttach, rightAttach;

  struct {
    std::shared_ptr<Shape> shape = nullptr;
    b2Vec2 localPoint = {0, 0};
    WeldJointConfig attachTemplate;
  } centerAttach;

  b2Vec2 rotationAxis;
  float girdleWidth;
  b2Rot initial3DRotation;
  float rotationSpeedRadPerSec;
};

class GirdleConnection : public Connection {
public:
  using Config = GirdleConnectionConfig;
  virtual void update(float dt) override;
  void rotateAroundAxis(float angle);
  void rotateAroundAxis(b2Rot rot);

protected:
  GirdleConnection(entt::registry &registry, const std::shared_ptr<World> world,
                   const GirdleConnectionConfig &config,
                   const std::shared_ptr<ShapeFactory> shapeFactory,
                   const std::shared_ptr<JointFactory> jointFactory);

  // Internal shapes
  std::shared_ptr<Circle> left;
  std::shared_ptr<Circle> right;
  std::shared_ptr<Circle> center;

  // Internal joints
  std::shared_ptr<PrismaticJoint> leftPrism;
  std::shared_ptr<PrismaticJoint> rightPrism;

  // External attachments
  std::shared_ptr<RevoluteJoint> leftAttach;
  std::shared_ptr<RevoluteJoint> rightAttach;
  std::shared_ptr<WeldJoint> centerAttach;

  float girdleWidth;
  b2Rot current3DRotation;
  b2Rot target3DRotation;
  float rotationSpeedRadPerSec;


private:
  void updateRotation(float dt);
  friend class ConnectionFactory;
};