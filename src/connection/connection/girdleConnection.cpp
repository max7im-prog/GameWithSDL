#include "girdleConnection.hpp"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include "miscUtils.hpp"
#include "prismaticJoint.hpp"
#include <stdexcept>
GirdleConnection::GirdleConnection(
    entt::registry &registry, const std::shared_ptr<World> world,
    const GirdleConnection::Config &config,
    const std::shared_ptr<ShapeFactory> shapeFactory,
    const std::shared_ptr<JointFactory> jointFactory)
    : Connection(registry, world), girdleWidth(config.girdleWidth),
      current3DRotationRad(config.initial3DRotationRad) {
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
      registerJoint(leftPrism, "leftPrism");
    }
    {
      auto cfg = prismCfg;
      cfg.jointDef.bodyIdB = config.rightAttach.shape->getBodyId();
      cfg.jointDef.localAnchorB = config.rightAttach.localPoint;
      cfg.jointDef.targetTranslation = config.girdleWidth / 2;
      rightPrism = jointFactory->create<PrismaticJoint>(cfg);
      registerJoint(rightPrism, "rightPrism");
    }
  }

  // Configure controllers
  rotationController = PIDRotController(config.rotationControlTemplate);
}

void GirdleConnection::Config::defaultConfig() {
  prismTemplate.defaultConfig();
  girdleWidth = 1;
  rotationAxis = {0, 1};
  initial3DRotationRad = 0.0f;
}

void GirdleConnection::update(float dt) {
  updateRotation(dt);
  Connection::update(dt);
}

void GirdleConnection::updateRotation(float dt) {
  if (auto locks = MiscUtils::lockAll(leftPrism, rightPrism)) {
    auto &[leftPrismLock, rightPrismLock] = *locks;

    constexpr float ROTATIONAL_SENSITIVITY = 0.01;
    if (std::abs(target3DRotationRad - current3DRotationRad) >
        ROTATIONAL_SENSITIVITY) {
      b2Rot current3DRotation = b2MakeRot(current3DRotationRad);
      b2Rot target3DRotation = b2MakeRot(target3DRotationRad);

      // Calculate the error
      b2Rot error = b2InvMulRot(current3DRotation, target3DRotation);
      b2Rot rotIncr = rotationController.update(error, dt);
      current3DRotationRad =
          b2Rot_GetAngle(b2MulRot(current3DRotation, rotIncr));

      // Change target offsets of the prismatic joints in a girdle
      float newOffset = girdleWidth / 2 * current3DRotation.c;
      rightPrismLock->setTargetTranslation(newOffset);
      leftPrismLock->setTargetTranslation(-newOffset);
    }
  }
}

void GirdleConnection::rotate3D(float angle) {
  target3DRotationRad = b2Rot_GetAngle(b2MakeRot(angle));
}

void GirdleConnection::rotate3D(b2Rot rot) {
  target3DRotationRad = b2Rot_GetAngle(rot);
}
