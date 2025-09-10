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

  struct {
    DistanceJointConfig templateJointCfg;
    CircleConfig leftShoulderTemplateConfig;
    CircleConfig rightShoulderTemplateConfig;
    CircleConfig centerTemplateConfig;
    DistanceJointConfig distanceJointTemplateConfig;
    RevoluteJointConfig leftAttachTemplateConfig;
    RevoluteJointConfig rightAttachTemplateConfig;
    WeldJointConfig centerAttachJointTemplateConfig;
  } configs;

  b2Vec2 rotationAxis;
  float girdleWidth;
  b2Rot initial3DRotation;
  struct {
    std::shared_ptr<Shape> shape = nullptr;
    b2Vec2 localPoint = {0, 0};
  } leftAttach, rightAttach, centerAttach;
};

class GirdleConnection : public Connection {
public:
  using Config = GirdleConnectionConfig;

  // virtual void update(float dt) override;
  // void rotateAroundAxis(float angle);
  // void rotateAroundAxis(b2Rot rot);

protected:
  GirdleConnection(entt::registry &registry, const std::shared_ptr<World> world,
                   const GirdleConnectionConfig &config,
                   const std::shared_ptr<ShapeFactory> shapeFactory,
                   const std::shared_ptr<JointFactory> jointFactory);

  // Internal shapes
  std::shared_ptr<Circle> leftShoulder;
  std::shared_ptr<Circle> rightShoulder;
  std::shared_ptr<Circle> center;

  // Internal joints
  std::shared_ptr<DistanceJoint> leftTopDistance;
  std::shared_ptr<DistanceJoint> leftBottomDistance;
  std::shared_ptr<DistanceJoint> rightTopDistance;
  std::shared_ptr<DistanceJoint> rightBottomDistance;
  std::shared_ptr<DistanceJoint> relativeDistance;

  // External attachments
  std::shared_ptr<RevoluteJoint> leftAttach;
  std::shared_ptr<RevoluteJoint> rightAttach;
  std::shared_ptr<WeldJoint> topCenterAttach;
  std::shared_ptr<WeldJoint> bottomCenterAttach;

  float girdleWidth;
  b2Rot current3DRotation;

private:
  friend class ConnectionFactory;
};