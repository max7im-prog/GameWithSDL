#include "girdleConnection.hpp"
#include "box2d/box2d.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include "circle.hpp"
#include "distanceJoint.hpp"
#include "emptyShape.hpp"
#include <stdexcept>

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
    auto cfg = config.centerTemplateConfig;
    cfg.bodyDef.position = globalCenter;
    center = shapeFactory->create<EmptyShape>(cfg);
  }
  {
    auto cfg = config.leftShoulderTemplateConfig;
    cfg.bodyDef.position = globalLeft;
    leftShoulder = shapeFactory->create<Circle>(cfg);
  }
  {
    auto cfg = config.rightShoulderTemplateConfig;
    cfg.bodyDef.position = globalRight;
    rightShoulder = shapeFactory->create<Circle>(cfg);
  }

  // Connect shapes TODO: implement
  {
    auto cfg = config.distanceJointTemplateConfig;
    cfg.jointDef.bodyIdA = center->getBodyId(); 
    cfg.jointDef.bodyIdB= leftShoulder->getBodyId(); 
    cfg.jointDef.localAnchorA = center->getLocalPoint(globalCenterAttachTop);
    cfg.jointDef.localAnchorB = {0,0};
    leftTopDistance = jointFactory->create<DistanceJoint>(cfg);
  }
  {
    auto cfg = config.distanceJointTemplateConfig;
    cfg.jointDef.bodyIdA = center->getBodyId(); 
    cfg.jointDef.bodyIdB= leftShoulder->getBodyId(); 
    cfg.jointDef.localAnchorA = center->getLocalPoint(globalCenterAttachBot);
    cfg.jointDef.localAnchorB = {0,0};
    leftBottomDistance = jointFactory->create<DistanceJoint>(cfg);
  }

  {
    auto cfg = config.distanceJointTemplateConfig;
    cfg.jointDef.bodyIdA = center->getBodyId(); 
    cfg.jointDef.bodyIdB= rightShoulder->getBodyId(); 
    cfg.jointDef.localAnchorA = center->getLocalPoint(globalCenterAttachTop);
    cfg.jointDef.localAnchorB = {0,0};
    rightBottomDistance= jointFactory->create<DistanceJoint>(cfg);
  }
  {
    auto cfg = config.distanceJointTemplateConfig;
    cfg.jointDef.bodyIdA = center->getBodyId(); 
    cfg.jointDef.bodyIdB= rightShoulder->getBodyId(); 
    cfg.jointDef.localAnchorA = center->getLocalPoint(globalCenterAttachBot);
    cfg.jointDef.localAnchorB = {0,0};
    rightBottomDistance= jointFactory->create<DistanceJoint>(cfg);
  }

  {
    auto cfg = config.distanceJointTemplateConfig;
    cfg.jointDef.bodyIdA = leftShoulder->getBodyId(); 
    cfg.jointDef.bodyIdB= rightShoulder->getBodyId(); 
    cfg.jointDef.localAnchorA = {0,0};
    cfg.jointDef.localAnchorB = {0,0};
    rightBottomDistance= jointFactory->create<DistanceJoint>(cfg);
  }

  
  
  // Connect external shapes TODO: implement
}