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
    : Connection(registry, world), girdleWidth(config._girdleWidth),
      current3DRotationRad(config._initial3DRotationRad) {
  if (!config._centerAttach._shape || !config._leftAttach._shape ||
      !config._rightAttach._shape) {
    throw std::invalid_argument("One of the attachments is invalid");
  }
  if (b2Length(config._rotationAxis) == 0) {
    throw std::invalid_argument("rotation axis has length of 0");
  }

  // Calculate stuff
  b2Vec2 rotPlaneVector =
      b2Normalize(b2RotateVector(b2MakeRot(-B2_PI / 2), config._rotationAxis));

  {
    auto prismCfg = config._prismTemplate;
    prismCfg.jointDef.bodyIdA = config._centerAttach._shape->getBodyId();
    prismCfg.jointDef.localAnchorA = config._centerAttach._localPoint;
    prismCfg.jointDef.localAxisA = rotPlaneVector;
    prismCfg.jointDef.enableLimit = true;
    prismCfg.jointDef.upperTranslation = config._girdleWidth / 2;
    prismCfg.jointDef.lowerTranslation = -config._girdleWidth / 2;
    prismCfg.jointDef.enableSpring = true;
    {
      auto cfg = prismCfg;
      cfg.jointDef.bodyIdB = config._leftAttach._shape->getBodyId();
      cfg.jointDef.localAnchorB = config._leftAttach._localPoint;
      cfg.jointDef.targetTranslation = -config._girdleWidth / 2;
      leftPrism = jointFactory->create<PrismaticJoint>(cfg);
      registerJoint(leftPrism, "leftPrism");
    }
    {
      auto cfg = prismCfg;
      cfg.jointDef.bodyIdB = config._rightAttach._shape->getBodyId();
      cfg.jointDef.localAnchorB = config._rightAttach._localPoint;
      cfg.jointDef.targetTranslation = config._girdleWidth / 2;
      rightPrism = jointFactory->create<PrismaticJoint>(cfg);
      registerJoint(rightPrism, "rightPrism");
    }
  }

  // Configure controllers
  rotationController = PIDRotController(config._rotationControlTemplate);
}

void GirdleConnection::Config::defaultConfig() {
  _prismTemplate.defaultConfig();
  _girdleWidth = 1;
  _rotationAxis = {0, 1};
  _initial3DRotationRad = 0.0f;
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
