#include "demoCreature.hpp"
#include "box2d/types.h"
#include "limbBody.hpp"
#include "physicsFactory.hpp"
#include "polygonBody.hpp"
#include "revoluteJoint.hpp"

void DemoCreature::move(b2Vec2 dir, float intensity) {}

DemoCreature::DemoCreature(entt::registry &registry,
                           const std::shared_ptr<World> world,
                           const DemoCreatureConfig &config,
                           const std::shared_ptr<PhysicsFactory> physicsFactory,
                           const std::shared_ptr<BodyFactory> bodyFactory)
    : Creature(registry, world) {

  // Calculate stuff
  constexpr size_t numSegments = 4;
  float segmentLen =
      b2Length(b2Vec2(config.sizeXMeters, config.sizeYMeters)) / numSegments;
  float segmentRadius = segmentLen / 8;
  float torsoWidth = config.sizeXMeters;
  float torsoHeight = config.sizeYMeters;
  auto groupId = PhysicsFactory::getNextNegativeId();

  // Create bodies
  auto torsoConfig = PolygonBodyConfig::defaultConfig();
  torsoConfig.polygonConfig.bodyDef.type = b2_dynamicBody;
  torsoConfig.polygonConfig.shapeDef.filter.groupIndex = groupId;
  torsoConfig.polygonConfig.vertices = {{(-torsoWidth / 2), (0)},
                                        {(-torsoWidth / 2), (torsoHeight)},
                                        {(torsoWidth / 2), (torsoHeight)},
                                        {(torsoWidth / 2), (0)}};

  auto limbConfig = LimbBodyConfig::defaultConfig();
  limbConfig.templateCapsuleConfig.bodyDef.type = b2_dynamicBody;
  limbConfig.templateCapsuleConfig.shapeDef.filter.groupIndex = groupId;

  for (size_t i = 0; i < numSegments; i++) {
    limbConfig.segments.push_back({.len = 0.25, .radius = segmentRadius});
  }
  {
    auto cfg = limbConfig;
    cfg.rotation = b2MakeRot(-B2_PI / 4);
    cfg.basePos =
        b2Add(config.position, b2Vec2(-torsoWidth / 2, torsoHeight / 2));
    leftArm = bodyFactory->createLimbBody(cfg);
    registerBodyPart(leftArm);
  }
  {
    auto cfg = limbConfig;
    cfg.rotation = b2MakeRot(-B2_PI / 4);
    cfg.basePos = b2Add(config.position, b2Vec2(-torsoWidth * 0.3, 0));
    leftLeg = bodyFactory->createLimbBody(cfg);
    registerBodyPart(leftLeg);
  }
  {
    auto cfg = limbConfig;
    cfg.rotation = b2MakeRot(B2_PI / 4);
    cfg.basePos =
        b2Add(config.position, b2Vec2(torsoWidth / 2, torsoHeight / 2));
    rightArm = bodyFactory->createLimbBody(cfg);
    registerBodyPart(rightArm);
  }
  {
    auto cfg = limbConfig;
    cfg.rotation = b2MakeRot(B2_PI / 4);
    cfg.basePos = b2Add(config.position, b2Vec2(torsoWidth * 0.3, 0));
    rightLeg = bodyFactory->createLimbBody(cfg);
    registerBodyPart(rightLeg);
  }
  {
    auto cfg = torsoConfig;
    cfg.polygonConfig.bodyDef.position = {config.position};
    torso = bodyFactory->createPolygonBody(cfg);
    registerBodyPart(torso);
  }

  // Create joints
  auto jointConfig = RevoluteJointConfig::defaultConfig();
  {
    auto cfg = jointConfig;
    cfg.jointDef.bodyIdA = torso->getPolygon()->getBodyId();
    cfg.jointDef.bodyIdB = leftLeg->getSegments()[0]->getBodyId();
    cfg.jointDef.localAnchorA = b2Vec2(-torsoWidth * 0.3, 0);
    cfg.jointDef.localAnchorB = {0, 0};
    leftHipJoint = physicsFactory->createRevoluteJoint(cfg);
    registerJoint(leftHipJoint);
  }
  {
    auto cfg = jointConfig;
    cfg.jointDef.bodyIdA = torso->getPolygon()->getBodyId();
    cfg.jointDef.bodyIdB = leftArm->getSegments()[0]->getBodyId();
    cfg.jointDef.localAnchorA = {-torsoWidth / 2, torsoHeight / 2};
    cfg.jointDef.localAnchorB = {0, 0};
    leftShoulderJoint = physicsFactory->createRevoluteJoint(cfg);
    registerJoint(leftShoulderJoint);
  }

  {
    auto cfg = jointConfig;
    cfg.jointDef.bodyIdA = torso->getPolygon()->getBodyId();
    cfg.jointDef.bodyIdB = rightLeg->getSegments()[0]->getBodyId();
    cfg.jointDef.localAnchorA = b2Vec2(torsoWidth * 0.3, 0);
    cfg.jointDef.localAnchorB = {0, 0};
    rightHipJoint = physicsFactory->createRevoluteJoint(cfg);
    registerJoint(rightHipJoint);
  }
  {
    auto cfg = jointConfig;
    cfg.jointDef.bodyIdA = torso->getPolygon()->getBodyId();
    cfg.jointDef.bodyIdB = rightArm->getSegments()[0]->getBodyId();
    cfg.jointDef.localAnchorA = {torsoWidth / 2, torsoHeight / 2};
    cfg.jointDef.localAnchorB = {0, 0};
    rightShoulderJoint = physicsFactory->createRevoluteJoint(cfg);
    registerJoint(rightShoulderJoint);
  }
}

DemoCreatureConfig DemoCreatureConfig::defaultConfig() {
  DemoCreatureConfig ret;
  ret.sizeXMeters = 1;
  ret.sizeYMeters = 1;
  ret.position = {0, 0};
  return ret;
}