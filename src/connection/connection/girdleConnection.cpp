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
    : Connection(registry, world) {
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
    auto cfg = config.configs.centerTemplateConfig;
    cfg.bodyDef.position = globalCenter;
    center = shapeFactory->create<Circle>(cfg);
    registerChild(center);
  }
  {
    auto cfg = config.configs.leftShoulderTemplateConfig;
    cfg.bodyDef.position = globalLeft;
    leftShoulder = shapeFactory->create<Circle>(cfg);
    registerChild(leftShoulder);
  }
  {
    auto cfg = config.configs.rightShoulderTemplateConfig;
    cfg.bodyDef.position = globalRight;
    rightShoulder = shapeFactory->create<Circle>(cfg);
    registerChild(rightShoulder);
  }

  // Connect shapes
  {
    auto cfg = config.configs.distanceJointTemplateConfig;
    cfg.jointDef.bodyIdA = center->getBodyId();
    cfg.jointDef.bodyIdB = leftShoulder->getBodyId();
    cfg.jointDef.localAnchorA = center->getLocalPoint(globalCenterAttachTop);
    cfg.jointDef.localAnchorB = {0, 0};
    cfg.jointDef.length = b2Distance(globalCenterAttachTop, globalLeft);
    leftTopDistance = jointFactory->create<DistanceJoint>(cfg);
    registerChild(leftTopDistance);
  }
  {
    auto cfg = config.configs.distanceJointTemplateConfig;
    cfg.jointDef.bodyIdA = center->getBodyId();
    cfg.jointDef.bodyIdB = leftShoulder->getBodyId();
    cfg.jointDef.localAnchorA = center->getLocalPoint(globalCenterAttachBot);
    cfg.jointDef.localAnchorB = {0, 0};

    cfg.jointDef.length = b2Distance(globalCenterAttachBot, globalLeft);
    leftBottomDistance = jointFactory->create<DistanceJoint>(cfg);
    registerChild(leftBottomDistance);
  }

  {
    auto cfg = config.configs.distanceJointTemplateConfig;
    cfg.jointDef.bodyIdA = center->getBodyId();
    cfg.jointDef.bodyIdB = rightShoulder->getBodyId();
    cfg.jointDef.localAnchorA = center->getLocalPoint(globalCenterAttachTop);
    cfg.jointDef.localAnchorB = {0, 0};
    cfg.jointDef.length = b2Distance(globalCenterAttachTop, globalRight);
    rightTopDistance = jointFactory->create<DistanceJoint>(cfg);

    registerChild(rightTopDistance);
  }
  {
    auto cfg = config.configs.distanceJointTemplateConfig;
    cfg.jointDef.bodyIdA = center->getBodyId();
    cfg.jointDef.bodyIdB = rightShoulder->getBodyId();
    cfg.jointDef.localAnchorA = center->getLocalPoint(globalCenterAttachBot);
    cfg.jointDef.localAnchorB = {0, 0};
    cfg.jointDef.length = b2Distance(globalCenterAttachBot, globalRight);
    rightBottomDistance = jointFactory->create<DistanceJoint>(cfg);
    registerChild(rightBottomDistance);
  }

  // Connect external
  {
    auto cfg = config.configs.leftAttachTemplateConfig;
    cfg.jointDef.bodyIdA = leftShoulder->getBodyId();
    cfg.jointDef.bodyIdB = config.leftAttach.shape->getBodyId();
    cfg.jointDef.localAnchorA = {0, 0};
    cfg.jointDef.localAnchorB = config.leftAttach.localPoint;
    leftAttach = jointFactory->create<RevoluteJoint>(cfg);
    registerChild(leftAttach);
  }
  {
    auto cfg = config.configs.rightAttachTemplateConfig;
    cfg.jointDef.bodyIdA = rightShoulder->getBodyId();
    cfg.jointDef.bodyIdB = config.rightAttach.shape->getBodyId();
    cfg.jointDef.localAnchorA = {0, 0};
    cfg.jointDef.localAnchorB = config.rightAttach.localPoint;
    rightAttach = jointFactory->create<RevoluteJoint>(cfg);
    registerChild(rightAttach);
  }
  {
    auto cfg = config.configs.centerAttachJointTemplateConfig;
    cfg.jointDef.bodyIdA = center->getBodyId();
    cfg.jointDef.bodyIdB = config.centerAttach.shape->getBodyId();
    cfg.jointDef.localAnchorA = {0,0};
    cfg.jointDef.localAnchorB =
        config.centerAttach.shape->getLocalPoint(globalCenter);
    topCenterAttach = jointFactory->create<WeldJoint>(cfg);
    registerChild(topCenterAttach);
  }
}

GirdleConnectionConfig GirdleConnectionConfig::defaultConfig() {
  GirdleConnectionConfig ret;

  // Joints
  ret.configs.centerAttachJointTemplateConfig =
      WeldJointConfig::defaultConfig();
  ret.configs.distanceJointTemplateConfig =
      DistanceJointConfig::defaultConfig();
  ret.configs.leftAttachTemplateConfig = RevoluteJointConfig::defaultConfig();
  ret.configs.rightAttachTemplateConfig = RevoluteJointConfig::defaultConfig();

  // Shapes
  ret.configs.centerTemplateConfig = CircleConfig::defaultConfig();
  ret.configs.leftShoulderTemplateConfig = CircleConfig::defaultConfig();
  ret.configs.rightShoulderTemplateConfig = CircleConfig::defaultConfig();

  // Other
  ret.girdleWidth = 1;
  ret.rotationAxis = {0, 1};
  ret.initial3DRotation = b2MakeRot(0);

  return ret;
}