#pragma once

#include "PIDRotController.hpp"
#include "box2d/math_functions.h"
#include "connection.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"
#include "world.hpp"
#include <memory>

class GirdleConnection : public Connection,
                         public VisitableImpl<GirdleConnection> {
public:
  struct Config : public Connection::Config {
    void defaultConfig() override;

    PrismaticJoint::Config _prismTemplate;
    PIDRotControllerConfig _rotationControlTemplate;

    BodyAttach _centerAttach;
    BodyAttach _leftAttach;
    BodyAttach _rightAttach;

    b2Vec2 _rotationAxis;
    float _girdleWidth;
    float _initial3DRotationRad;
  };
  virtual void update(float dt) override;
  void rotate3D(float angle);
  void rotate3D(b2Rot rot);

protected:
  GirdleConnection(entt::registry &registry, const std::shared_ptr<World> world,
                   const GirdleConnection::Config &config,
                   const std::shared_ptr<ShapeFactory> shapeFactory,
                   const std::shared_ptr<JointFactory> jointFactory);

  std::weak_ptr<PrismaticJoint> leftPrism;
  std::weak_ptr<PrismaticJoint> rightPrism;

  float girdleWidth;
  float current3DRotationRad;
  float target3DRotationRad;
  float rotationSpeedRadPerSec;

  PIDRotController rotationController;

private:
  void updateRotation(float dt);
  friend class ConnectionFactory;
};
