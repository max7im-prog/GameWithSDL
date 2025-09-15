#include "girdleConnection.hpp"
#include "box2d/box2d.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include "circle.hpp"
#include "distanceJoint.hpp"
#include "emptyShape.hpp"
#include "revoluteJoint.hpp"
#include "weldJoint.hpp"
#include <stdexcept>
// TODO: add filter configuration
// TODO: add rotate() and update() functions
GirdleConnection::GirdleConnection(
    entt::registry &registry, const std::shared_ptr<World> world,
    const GirdleConnectionConfig &config,
    const std::shared_ptr<ShapeFactory> shapeFactory,
    const std::shared_ptr<JointFactory> jointFactory)
    : Connection(registry, world), girdleWidth(config.girdleWidth),
      current3DRotation(config.initial3DRotation) {
  if (!config.centerAttach.shape || !config.rightAttach.shape ||
      !config.leftAttach.shape) {
    throw std::invalid_argument("One of the attachments is missing");
  }
  if (b2Length(config.rotationAxis) == 0) {
    throw std::invalid_argument("rotation axis has length of 0");
  }

  // Calculate stuff
  b2Vec2 globalCenter =
      config.centerAttach.shape->getWorldPoint(config.centerAttach.localPoint);
  b2Vec2 normRotAxis = b2Normalize(config.rotationAxis);
  b2Vec2 incr = b2RotateVector(b2MakeRot(-B2_PI / 2), normRotAxis);
  b2Vec2 globalLeft =
      b2Add(globalCenter, b2MulSV(-config.girdleWidth / 2, incr));
  b2Vec2 globalRight =
      b2Add(globalCenter, b2MulSV(config.girdleWidth / 2, incr));
  b2Vec2 globalCenterAttachTop =
      b2Add(globalCenter, b2MulSV(config.girdleWidth / 6, normRotAxis));
  b2Vec2 globalCenterAttachBot =
      b2Add(globalCenter, b2MulSV(-config.girdleWidth / 6, normRotAxis));

  // Create shapes
  {
    auto cfg = config.configs.centerTemplate;
    cfg.bodyDef.position = globalCenter;
    cfg.shapeDef.filter = config.filter;
    center = shapeFactory->create<Circle>(cfg);
    registerChild(center);
  }
  {
    auto cfg = config.configs.leftTemplate;
    cfg.bodyDef.position = globalLeft;
    cfg.shapeDef.filter = config.filter;
    left = shapeFactory->create<Circle>(cfg);
    registerChild(left);
  }
  {
    auto cfg = config.configs.rightTemplate;
    cfg.bodyDef.position = globalRight;
    cfg.shapeDef.filter = config.filter;
    right = shapeFactory->create<Circle>(cfg);
    registerChild(right);
  }

  // Connect shapes
  {
    auto prismCfg = config.configs.prismTemplate;
    prismCfg.jointDef.bodyIdA = center->getBodyId();
    prismCfg.jointDef.localAnchorA = {0, 0};
    prismCfg.jointDef.localAxisA = {1, 0};
    prismCfg.jointDef.enableLimit = true;
    prismCfg.jointDef.upperTranslation = config.girdleWidth / 2;
    prismCfg.jointDef.lowerTranslation = -config.girdleWidth / 2;
    prismCfg.jointDef.enableSpring = true;
    {
      auto cfg = prismCfg;
      cfg.jointDef.bodyIdB = left->getBodyId();
      cfg.jointDef.localAnchorB = {0, 0};
      cfg.jointDef.targetTranslation = -config.girdleWidth / 2;
      leftPrism = jointFactory->create<PrismaticJoint>(cfg);
      registerChild(leftPrism);
    }
    {
      auto cfg = prismCfg;
      cfg.jointDef.bodyIdB = right->getBodyId();
      cfg.jointDef.localAnchorB = {0, 0};
      cfg.jointDef.targetTranslation = config.girdleWidth / 2;
      rightPrism = jointFactory->create<PrismaticJoint>(cfg);
      registerChild(rightPrism);
    }
  }
  // Connect external
  {
    auto cfg = config.leftAttach.attachTemplate;
    cfg.jointDef.bodyIdA = left->getBodyId();
    cfg.jointDef.bodyIdB = config.leftAttach.shape->getBodyId();
    cfg.jointDef.localAnchorA = {0, 0};
    cfg.jointDef.localAnchorB = config.leftAttach.localPoint;
    leftAttach = jointFactory->create<RevoluteJoint>(cfg);
    registerChild(leftAttach);
  }
  {
    auto cfg = config.rightAttach.attachTemplate;
    cfg.jointDef.bodyIdA = right->getBodyId();
    cfg.jointDef.bodyIdB = config.rightAttach.shape->getBodyId();
    cfg.jointDef.localAnchorA = {0, 0};
    cfg.jointDef.localAnchorB = config.rightAttach.localPoint;
    rightAttach = jointFactory->create<RevoluteJoint>(cfg);
    registerChild(rightAttach);
  }
  {
    auto cfg = config.centerAttach.attachTemplate;
    cfg.jointDef.bodyIdA = center->getBodyId();
    cfg.jointDef.bodyIdB = config.centerAttach.shape->getBodyId();
    cfg.jointDef.localAnchorA = {0, 0};
    cfg.jointDef.localAnchorB =
        config.centerAttach.shape->getLocalPoint(globalCenter);
    centerAttach = jointFactory->create<WeldJoint>(cfg);
    registerChild(centerAttach);
  }

  // Configure controllers
  rotationController = PIDRotController(config.configs.rotationControlTemplate);
}

GirdleConnectionConfig GirdleConnectionConfig::defaultConfig() {
  GirdleConnectionConfig ret;

  // Joints
  ret.centerAttach.attachTemplate = WeldJointConfig::defaultConfig();
  ret.leftAttach.attachTemplate = RevoluteJointConfig::defaultConfig();
  ret.rightAttach.attachTemplate = RevoluteJointConfig::defaultConfig();
  ret.configs.prismTemplate = PrismaticJointConfig::defaultConfig();

  // Shapes
  ret.configs.centerTemplate = CircleConfig::defaultConfig();
  ret.configs.centerTemplate.bodyDef.type = b2_dynamicBody;
  ret.configs.leftTemplate = CircleConfig::defaultConfig();
  ret.configs.leftTemplate.bodyDef.type = b2_dynamicBody;
  ret.configs.rightTemplate = CircleConfig::defaultConfig();
  ret.configs.rightTemplate.bodyDef.type = b2_dynamicBody;

  // Other
  ret.girdleWidth = 1;
  ret.rotationAxis = {0, 1};
  ret.initial3DRotation = b2MakeRot(0);
  ret.filter = b2DefaultFilter();
  ret.configs.rotationControlTemplate.kp = 1;

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
    b2Rot rotIncr = rotationController.update(error,dt);
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

void GirdleConnection::rotateAroundAxis(b2Rot rot) { target3DRotation = rot; }