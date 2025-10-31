#pragma once

#include "PIDRotController.hpp"
#include "box2d/math_functions.h"
#include "connection.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"
#include "world.hpp"
#include <memory>

struct GirdleConnectionConfig : public ConnectionConfig {
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;

  PrismaticJointConfig prismTemplate;
  PIDRotControllerConfig rotationControlTemplate;

  struct {
    std::shared_ptr<Shape> shape = nullptr;
    b2Vec2 localPoint = {0, 0};
  } centerAttach, leftAttach,rightAttach;

  b2Vec2 rotationAxis;
  float girdleWidth;
  b2Rot initial3DRotation;
};

class GirdleConnection : public Connection , public Visitable<GirdleConnection>{
public:
  using Config = GirdleConnectionConfig;
  virtual void update(float dt) override;
  void rotate3D(float angle);
  void rotate3D(b2Rot rot);

protected:
  GirdleConnection(entt::registry &registry, const std::shared_ptr<World> world,
                   const GirdleConnectionConfig &config,
                   const std::shared_ptr<ShapeFactory> shapeFactory,
                   const std::shared_ptr<JointFactory> jointFactory);

  std::weak_ptr<PrismaticJoint> leftPrism;
  std::weak_ptr<PrismaticJoint> rightPrism;

  float girdleWidth;
  b2Rot current3DRotation;
  b2Rot target3DRotation;
  float rotationSpeedRadPerSec;

  PIDRotController rotationController;

private:
  void updateRotation(float dt);
  friend class ConnectionFactory;
};