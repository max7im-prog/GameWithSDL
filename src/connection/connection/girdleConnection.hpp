#pragma once

#include "box2d/box2d.h"
#include "box2d/collision.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include "capsule.hpp"
#include "circle.hpp"
#include "connection.hpp"
#include "distanceJoint.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"
// #include "body.hpp"

struct GirdleConnectionConfig : public ConnectionConfig {
  GirdleConnectionConfig defaultConfig();
  DistanceJointConfig templateJointCfg;
  CircleConfig leftShoulderTemplateConfig;
  CircleConfig rightShoulderTemplateConfig;
  b2Vec2 globalCenterPoint;
  b2Vec2 rotationAxis;
  float limbDistance;
  float initialRotation;
  struct{
  // std::shared_ptr<Body> body;
  }leftAttach, rightAttach;
};

class GirdleConnection : public Connection {
public:
  using Config = GirdleConnectionConfig;

  virtual void update(float dt) override;
protected:
  GirdleConnection(entt::registry &registry, const std::shared_ptr<World> world,
                   const GirdleConnectionConfig &config,
                   const std::shared_ptr<ShapeFactory> shapeFactory,
                   const std::shared_ptr<JointFactory> jointFactory);

  std::shared_ptr<Circle> leftShoulder;
  std::shared_ptr<Circle> rightShoulder;
  std::shared_ptr<Capsule> center;
  std::shared_ptr<DistanceJoint> leftTopDistance;
  std::shared_ptr<DistanceJoint> leftBottomDistance;
  std::shared_ptr<DistanceJoint> rightTopDistance;
  std::shared_ptr<DistanceJoint> rightBottomDistance;
  std::shared_ptr<DistanceJoint> relativeDistance;

private:
};