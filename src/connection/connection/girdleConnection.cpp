#include "girdleConnection.hpp"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include <stdexcept>
GirdleConnection::GirdleConnection(
    entt::registry &registry, const std::shared_ptr<World> world,
    const GirdleConnectionConfig &config,
    const std::shared_ptr<ShapeFactory> shapeFactory,
    const std::shared_ptr<JointFactory> jointFactory)
    : Connection(registry, world), girdleWidth(config.girdleWidth),
      current3DRotation(config.initial3DRotation) {
  if (!config.centerAttach.shape || !config.leftAttach.shape ||
      !config.rightAttach.shape) {
    throw std::invalid_argument("One of the attachments is invalid");
  }
  if (b2Length(config.rotationAxis) == 0) {
    throw std::invalid_argument("rotation axis has length of 0");
  }

  // Calculate stuff
  b2Vec2 rotPlaneVector =
      b2Normalize(b2RotateVector(b2MakeRot(-B2_PI / 2), config.rotationAxis));

  {
    auto prismCfg = config.prismTemplate;
    prismCfg.jointDef.bodyIdA = config.centerAttach.shape->getBodyId();
    prismCfg.jointDef.localAnchorA = config.centerAttach.localPoint;
    prismCfg.jointDef.localAxisA = rotPlaneVector;
    prismCfg.jointDef.enableLimit = true;
    prismCfg.jointDef.upperTranslation = config.girdleWidth / 2;
    prismCfg.jointDef.lowerTranslation = -config.girdleWidth / 2;
    prismCfg.jointDef.enableSpring = true;
    {
      auto cfg = prismCfg;
      cfg.jointDef.bodyIdB = config.leftAttach.shape->getBodyId();
      cfg.jointDef.localAnchorB = config.leftAttach.localPoint;
      cfg.jointDef.targetTranslation = -config.girdleWidth / 2;
      leftPrism = jointFactory->create<PrismaticJoint>(cfg);
      registerChild(leftPrism);
    }
    {
      auto cfg = prismCfg;
      cfg.jointDef.bodyIdB = config.rightAttach.shape->getBodyId();
      cfg.jointDef.localAnchorB = config.rightAttach.localPoint;
      cfg.jointDef.targetTranslation = config.girdleWidth / 2;
      rightPrism = jointFactory->create<PrismaticJoint>(cfg);
      registerChild(rightPrism);
    }
  }

  // Configure controllers
  rotationController = PIDRotController(config.rotationControlTemplate);
}

GirdleConnectionConfig GirdleConnectionConfig::defaultConfig() {
  GirdleConnectionConfig ret;
  ret.prismTemplate = PrismaticJointConfig::defaultConfig();
  ret.girdleWidth = 1;
  ret.rotationAxis = {0, 1};
  ret.initial3DRotation = b2MakeRot(0);
  return ret;
}

void GirdleConnection::update(float dt) {
  updateRotation(dt);
  Connection::update(dt);
}

void GirdleConnection::updateRotation(float dt) {
  constexpr float ROTATIONAL_SENSITIVITY = 0.01;
  if (std::abs(b2Rot_GetAngle(target3DRotation) -
               b2Rot_GetAngle(current3DRotation)) > ROTATIONAL_SENSITIVITY) {

    // Calculate the error
    b2Rot error = b2InvMulRot(current3DRotation, target3DRotation);
    b2Rot rotIncr = rotationController.update(error, dt);
    current3DRotation = b2MulRot(current3DRotation, rotIncr);

    // Change target offsets of the prismatic joints in a girdle
    float newOffset = girdleWidth / 2 * current3DRotation.c;
    rightPrism->setTargetTranslation(newOffset);
    leftPrism->setTargetTranslation(-newOffset);
  }
}

void GirdleConnection::rotate3D(float angle) {
  target3DRotation = b2MakeRot(angle);
}

void GirdleConnection::rotate3D(b2Rot rot) { target3DRotation = rot; }