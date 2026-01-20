#include "girdleConnection.hpp"
#include "body.hpp"
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

  if (b2Length(config._rotationAxis) == 0) {
    throw std::invalid_argument(
        "GirdleConnection: rotation axis has length of 0");
  }

  // Check bodies
  std::shared_ptr<Body> centerBody;
  std::shared_ptr<Body> leftBody;
  std::shared_ptr<Body> rightBody;
  centerBody = config._centerAttach._body.lock();
  leftBody = config._leftAttach._body.lock();
  rightBody = config._rightAttach._body.lock();

  if (!centerBody) {
    spdlog::error("GirdleConnection: center body expired");
    throw std::runtime_error("GirdleConnection: center body expired");
  }

  if (!leftBody) {
    spdlog::error("GirdleConnection: left body expired");
    throw std::runtime_error("GirdleConnection: left body expired");
  }

  if (!rightBody) {
    spdlog::error("GirdleConnection: right body expired");
    throw std::runtime_error("GirdleConnection: right body expired");
  }

  // Check if shapes exist
  std::shared_ptr<Shape> centerShape;
  std::shared_ptr<Shape> leftShape;
  std::shared_ptr<Shape> rightShape;
  {
    const auto &shapes = centerBody->getShapes();
    if (shapes.contains(config._centerAttach._shapeName)) {
      centerShape = shapes.at(config._centerAttach._shapeName).lock();
    } else {
      spdlog::error("GirdleConnection: center body has no shape named '{}'",
                    config._centerAttach._shapeName);
      throw std::runtime_error(
          "GirdleConnection: center body has no shape named '" +
          config._centerAttach._shapeName + "'");
    }
  }
  {
    const auto &shapes = leftBody->getShapes();
    if (shapes.contains(config._leftAttach._shapeName)) {
      leftShape = shapes.at(config._leftAttach._shapeName).lock();
    } else {
      spdlog::error("GirdleConnection: left body has no shape named '{}'",
                    config._leftAttach._shapeName);
      throw std::runtime_error(
          "GirdleConnection: left body has no shape named '" +
          config._leftAttach._shapeName + "'");
    }
  }
  {
    const auto &shapes = rightBody->getShapes();
    if (shapes.contains(config._rightAttach._shapeName)) {
      rightShape = shapes.at(config._rightAttach._shapeName).lock();
    } else {
      spdlog::error("GirdleConnection: right body has no shape named '{}'",
                    config._rightAttach._shapeName);
      throw std::runtime_error(
          "GirdleConnection: right body has no shape named '" +
          config._rightAttach._shapeName + "'");
    }
  }

  // Check if shapes are locked
  if (!centerShape) {
    throw std::runtime_error(
        "GirdleConnection: central shape expired on createion");
  }
  if (!leftShape) {
    throw std::runtime_error(
        "GirdleConnection: left shape expired on createion");
  }
  if (!rightShape) {
    throw std::runtime_error(
        "GirdleConnection: right shape expired on createion");
  }

  // Calculate stuff
  b2Vec2 rotPlaneVector =
      b2Normalize(b2RotateVector(b2MakeRot(-B2_PI / 2), config._rotationAxis));

  {
    auto prismCfg = config._prismTemplate;
    prismCfg.jointDef.bodyIdA = centerShape->getBodyId();
    prismCfg.jointDef.localAnchorA = config._centerAttach._shapeLocalPoint;
    prismCfg.jointDef.localAxisA = rotPlaneVector;
    prismCfg.jointDef.enableLimit = true;
    prismCfg.jointDef.upperTranslation = config._girdleWidth / 2;
    prismCfg.jointDef.lowerTranslation = -config._girdleWidth / 2;
    prismCfg.jointDef.enableSpring = true;
    {
      auto cfg = prismCfg;
      cfg.jointDef.bodyIdB = leftShape->getBodyId();
      cfg.jointDef.localAnchorB = config._leftAttach._shapeLocalPoint;
      cfg.jointDef.targetTranslation = -config._girdleWidth / 2;
      leftPrism = jointFactory->create<PrismaticJoint>(cfg);
      registerJoint(leftPrism, "leftPrism");
    }
    {
      auto cfg = prismCfg;
      cfg.jointDef.bodyIdB = rightShape->getBodyId();
      cfg.jointDef.localAnchorB = config._rightAttach._shapeLocalPoint;
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
