#include "demoCreature.hpp"
#include "PIDScalarController.hpp"
#include "box2d/box2d.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include "creature.hpp"
#include "girdleConnection.hpp"
#include "limbBody.hpp"
#include "physicsUtils.hpp"
#include "polygonBody.hpp"
#include "world.hpp"
#include <chrono>
#include <sys/types.h>

void DemoCreature::move(b2Vec2 dir, float intensity) {
  moveContext.intensity = intensity;
  moveContext.dir = dir;
  moveContext.move = true;
}

DemoCreature::DemoCreature(
    entt::registry &registry, const std::shared_ptr<World> world,
    const DemoCreatureConfig &config,
    const std::shared_ptr<BodyFactory> bodyFactory,
    const std::shared_ptr<ConnectionFactory> connectionFactory)
    : Creature(registry, world) {

  // Calculate stuff
  constexpr size_t numSegments = 2;
  float segmentLen = b2Length(b2Vec2(config.sizeXMeters, config.sizeYMeters)) *
                     0.7f / numSegments;
  float segmentRadius = segmentLen / 16;
  float torsoWidth = config.sizeXMeters;
  float torsoHeight = config.sizeYMeters;
  auto groupId = ShapeFactory::getNextNegativeId();

  b2Vec2 leftShoulderPos =
      b2Add(config.position, b2Vec2(-torsoWidth * 0.5, torsoHeight / 2));
  b2Vec2 rightShoulderPos =
      b2Add(config.position, b2Vec2(torsoWidth * 0.5, torsoHeight / 2));
  b2Vec2 leftHipPos = b2Add(config.position, b2Vec2(-torsoWidth * 0.3, 0));
  b2Vec2 rightHipPos = b2Add(config.position, b2Vec2(torsoWidth * 0.3, 0));

  // Create bodies
  auto torsoConfig = PolygonBodyConfig::defaultConfig();
  torsoConfig.shapeCfg.bodyDef.type = b2_dynamicBody;
  torsoConfig.shapeCfg.shapeDef.filter = CreatureConfig::defaultFilter();
  torsoConfig.shapeCfg.shapeDef.filter.groupIndex = groupId;
  torsoConfig.shapeCfg.vertices = {{(-torsoWidth / 4), (0)},
                                   {(-torsoWidth / 4), (torsoHeight)},
                                   {(torsoWidth / 4), (torsoHeight)},
                                   {(torsoWidth / 4), (0)}};

  auto shoulderConfig = CircleBodyConfig::defaultConfig();
  shoulderConfig.shapeCfg.bodyDef.type = b2_dynamicBody;
  shoulderConfig.shapeCfg.radius = segmentRadius * 4;
  shoulderConfig.shapeCfg.shapeDef.filter = CreatureConfig::defaultFilter();
  shoulderConfig.shapeCfg.shapeDef.filter.groupIndex = groupId;

  auto limbConfig = LimbBodyConfig::defaultConfig();
  limbConfig.templateCapsuleConfig.bodyDef.type = b2_dynamicBody;
  limbConfig.templateCapsuleConfig.shapeDef.filter =
      CreatureConfig::defaultFilter();
  limbConfig.templateCapsuleConfig.shapeDef.filter.groupIndex = groupId;
  limbConfig.limbControlConfig = {.KPMultiplier = 7.0f,
                                  .KIMultiplier = 0.3f,
                                  .KDMultiplier = 10.0f,
                                  .maxForceMultiplier = 15.0f};

  limbConfig.rootRot = b2MakeRot(-B2_PI/2);

  

  // Limbs
  {
    auto cfg = limbConfig;
    cfg.segments.clear();
    cfg.basePos =
        b2Add(config.position, b2Vec2(-torsoWidth * 0.5, torsoHeight / 2));
    {

      auto lastPos = cfg.basePos;
      b2Rot baseRot = b2MakeRot(-B2_PI *3/ 4);
      b2Rot incrRot = b2MakeRot(0);
      b2Rot q = baseRot;

      for (size_t i = 0; i < numSegments; i++) {
        auto incr = b2MulSV(segmentLen, b2Vec2(1,0));
        q = b2MulRot(q, incrRot);
        incr = b2RotateVector(q, incr);
        auto newPos = b2Add(incr, lastPos);
        cfg.segments.push_back({.endPos = newPos, .radius = segmentRadius});
        lastPos = newPos;
      }
    }
    // cfg.initialAngleConstraints = {{.minRot = -B2_PI/2,.maxRot = B2_PI/2,.enable = true},{.minRot = -B2_PI/2,.maxRot = B2_PI/2,.enable = true}};
    cfg.initialAngleConstraints = std::vector<AngleConstraint>(numSegments);
    leftArm = bodyFactory->create<LimbBody>(cfg);
    registerChild(leftArm);
  }
  {
    auto cfg = limbConfig;
    cfg.segments.clear();
    cfg.basePos = b2Add(config.position, b2Vec2(-torsoWidth * 0.3, 0));
    {
      auto lastPos = cfg.basePos;
      b2Rot baseRot = b2MakeRot(-B2_PI *3/ 4);
      b2Rot incrRot = b2MakeRot(0);
      b2Rot q = baseRot;

      for (size_t i = 0; i < numSegments; i++) {
        auto incr = b2MulSV(segmentLen, b2Vec2(1,0));
        q = b2MulRot(q, incrRot);
        incr = b2RotateVector(q, incr);
        auto newPos = b2Add(incr, lastPos);
        cfg.segments.push_back({.endPos = newPos, .radius = segmentRadius});
        lastPos = newPos;
      }
    }
    cfg.initialAngleConstraints = std::vector<AngleConstraint>(numSegments);
    leftLeg = bodyFactory->create<LimbBody>(cfg);
    registerChild(leftLeg);
  }
  {
    auto cfg = limbConfig;
    cfg.basePos =
        b2Add(config.position, b2Vec2(torsoWidth * 0.5, torsoHeight / 2));
    {
      auto lastPos = cfg.basePos;
      for (size_t i = 0; i < numSegments; i++) {
        auto incr = b2MulSV(segmentLen, b2Vec2(1,0));
        b2Rot q = b2MakeRot(-B2_PI / 4);
        incr = b2RotateVector(q, incr);
        auto newPos = b2Add(incr, lastPos);
        cfg.segments.push_back({.endPos = newPos, .radius = segmentRadius});
        lastPos = newPos;
      }
    }
    cfg.initialAngleConstraints = std::vector<AngleConstraint>(numSegments);
    rightArm = bodyFactory->create<LimbBody>(cfg);
    registerChild(rightArm);
  }
  {
    auto cfg = limbConfig;
    cfg.basePos = b2Add(config.position, b2Vec2(torsoWidth * 0.3, 0));
    {
      auto lastPos = cfg.basePos;
      for (size_t i = 0; i < numSegments; i++) {
        auto incr = b2MulSV(segmentLen, b2Vec2(1,0));
        b2Rot q = b2MakeRot(-B2_PI / 4);
        incr = b2RotateVector(q, incr);
        auto newPos = b2Add(incr, lastPos);
        cfg.segments.push_back({.endPos = newPos, .radius = segmentRadius});
        lastPos = newPos;
      }
    }
    cfg.initialAngleConstraints = std::vector<AngleConstraint>(numSegments);
    cfg.limbControlConfig.KPMultiplier = 100;
    cfg.limbControlConfig.KDMultiplier = 100;
    rightLeg = bodyFactory->create<LimbBody>(cfg);
    registerChild(rightLeg);
  }

  // Torso
  {
    auto cfg = torsoConfig;
    cfg.shapeCfg.bodyDef.position = {config.position};
    torso = bodyFactory->create<PolygonBody>(cfg);
    registerChild(torso);
  }

  // Shoulders and hips
  {
    auto cfg = shoulderConfig;
    cfg.shapeCfg.bodyDef.position = leftShoulderPos;
    leftShoulder = bodyFactory->create<CircleBody>(cfg);
    registerChild(leftShoulder);
  }
  {
    auto cfg = shoulderConfig;
    cfg.shapeCfg.bodyDef.position = rightShoulderPos;
    rightShoulder = bodyFactory->create<CircleBody>(cfg);
    registerChild(rightShoulder);
  }
  {
    auto cfg = shoulderConfig;
    cfg.shapeCfg.bodyDef.position = leftHipPos;
    leftHip = bodyFactory->create<CircleBody>(cfg);
    registerChild(leftHip);
  }
  {
    auto cfg = shoulderConfig;
    cfg.shapeCfg.bodyDef.position = rightHipPos;
    rightHip = bodyFactory->create<CircleBody>(cfg);
    registerChild(rightHip);
  }

  // Create connections

  // Hips
  {
    auto cfg = GirdleConnectionConfig::defaultConfig();
    cfg.girdleWidth = torsoWidth * 0.4;
    cfg.centerAttach.shape = torso->getPolygon();
    cfg.centerAttach.localPoint = {0, 0};

    cfg.leftAttach.shape = leftHip->getCircle();
    cfg.leftAttach.localPoint = {0, 0};

    cfg.rightAttach.shape = rightHip->getCircle();
    cfg.rightAttach.localPoint = {0, 0};

    cfg.rotationControlTemplate.kp = 1.0f;
    cfg.rotationControlTemplate.kd = 0.0f;
    cfg.rotationControlTemplate.ki = 0.0f;

    cfg.rotationAxis = {0, 1};
    cfg.prismTemplate.jointDef.hertz = 10;

    hipConnection = connectionFactory->create<GirdleConnection>(cfg);
    registerChild(hipConnection);
  }

  leftLeg->connect(connectionFactory, leftHip->getCircle(), {0, 0});
  rightLeg->connect(connectionFactory, rightHip->getCircle(), {0, 0});

  // Shoulders
  {
    auto cfg = GirdleConnectionConfig::defaultConfig();
    cfg.girdleWidth = torsoWidth * 0.8f;
    cfg.centerAttach.shape = torso->getPolygon();
    cfg.centerAttach.localPoint = {0, torsoHeight / 2.0f};

    cfg.leftAttach.shape = leftShoulder->getCircle();
    cfg.leftAttach.localPoint = {0, 0};

    cfg.rightAttach.shape = rightShoulder->getCircle();
    cfg.rightAttach.localPoint = {0, 0};

    cfg.rotationControlTemplate.kp = 1.0f;
    cfg.rotationControlTemplate.kd = 0.0f;
    cfg.rotationControlTemplate.ki = 0.0f;

    cfg.rotationAxis = {0, 1};
    cfg.prismTemplate.jointDef.hertz = 10;

    shoulderConnection = connectionFactory->create<GirdleConnection>(cfg);
    registerChild(shoulderConnection);
  }

  leftArm->connect(connectionFactory, leftShoulder->getCircle(), {0, 0});
  rightArm->connect(connectionFactory, rightShoulder->getCircle(), {0, 0});

  // Configure controllers
  {
    float inertia = torso->getPolygon()->getRotationalInertia();
    PIDScalarControllerConfig cfg = {.kp = inertia * 160.0f,
                                     .ki = 0.0f,
                                     .kd = inertia * 49.0f,
                                     .maxForce = inertia * 150};
    torsoAngleController = PIDScalarController(cfg);
  }
  {
    float force = torso->getPolygon()->getMass() *
                  b2Length(b2World_GetGravity(world->getWorldId()));
    PIDScalarControllerConfig cfg = {.kp = force * 2.0f,
                                     .ki = force * 0.5f,
                                     .kd = force * 16.0f,
                                     .maxForce = force * 3};
    leftLegHeightController = PIDScalarController(cfg);
    rightLegHeightController = PIDScalarController(cfg);
  }
  {
    float force = torso->getPolygon()->getMass() *
                  b2Length(b2World_GetGravity(world->getWorldId()));
    PIDScalarControllerConfig cfg = {.kp = force * 5.0f,
                                     .ki = 0.0f,
                                     .kd = force * 16.0f,
                                     .maxForce = force * 3};
    horizontalDampingController = PIDScalarController(cfg);
  }
  {
    float force = torso->getPolygon()->getMass() *
                  b2Length(b2World_GetGravity(world->getWorldId()));
    PIDScalarControllerConfig cfg = {.kp = force * 5.0f,
                                     .ki = 0.0f,
                                     .kd = force * 16.0f,
                                     .maxForce = force * 10};
    horizontalSpeedController = PIDScalarController(cfg);
  }

  // Assign values
  legHeight = segmentLen * numSegments * 0.9;
  creatureState = CreatureState::ON_GROUND;
  creatureAbilities = CreatureAbilities::CAN_JUMP;
  moveContext.maxSpeedMultiplier = 3;
  moveContext.defaultSpeedMpS = 5;
}

DemoCreatureConfig DemoCreatureConfig::defaultConfig() {
  DemoCreatureConfig ret;
  ret.sizeXMeters = 1;
  ret.sizeYMeters = 1;
  ret.position = {0, 0};
  return ret;
}

void DemoCreature::aim(b2Vec2 worldPoint, bool aim) {
  leftArm->setTracking(worldPoint, aim);
  rightArm->setTracking(worldPoint, aim);
}

void DemoCreature::update(float dt) {
  keepTorsoUpright(dt);
  keepTorsoAboveTheGround(dt);
  dampHorizontalMovement(dt);
  updateJump(dt);
  updateMove(dt);
  updateFeet(dt);
  Creature::update(dt);
}

void DemoCreature::keepTorsoUpright(float dt) {
  float error = -b2Rot_GetAngle(torso->getPolygon()->getRotation());
  float torque = torsoAngleController.update(error, dt);
  torso->getPolygon()->applyTorque(torque);
}

void DemoCreature::keepTorsoAboveTheGround(float dt) {
  b2Vec2 translation = {0, -legHeight};
  b2QueryFilter filter = b2DefaultQueryFilter();
  filter.maskBits = filter.maskBits & (~ObjectCategory::CREATURE);

  // Left leg
  {
    b2Vec2 castOrigin = leftLeg->getBasePos();
    auto res = b2World_CastRayClosest(world->getWorldId(), castOrigin,
                                      {0, -legHeight * 3}, filter);
    if (res.hit) {
      float realHeight = b2Length(b2Sub(res.point, castOrigin));
      if (realHeight < legHeight * 1.2) {
        float error = legHeight - realHeight;
        float force = leftLegHeightController.update(error, dt);
        b2Body_ApplyForce(torso->getPolygon()->getBodyId(), {0, force},
                          castOrigin, true);
      }
    } else {
      leftLegHeightController.reset();
    }
  }

  // Right leg
  {
    b2Vec2 castOrigin = rightLeg->getBasePos();
    auto res = b2World_CastRayClosest(world->getWorldId(), castOrigin,
                                      {0, -legHeight * 3}, filter);
    if (res.hit) {
      float realHeight = b2Length(b2Sub(res.point, castOrigin));
      if (realHeight < legHeight * 1.2) {
        float error = legHeight - realHeight;
        float force = rightLegHeightController.update(error, dt);
        b2Body_ApplyForce(torso->getPolygon()->getBodyId(), {0, force},
                          castOrigin, true);
      }
    } else {
      rightLegHeightController.reset();
    }
  }
}

void DemoCreature::dampHorizontalMovement(float dt) {
  if (jumpContext.jumpState == JumpContext::JumpState::ON_GROUND) {
    auto bodyId = torso->getPolygon()->getBodyId();
    auto error = 0 - b2Body_GetLinearVelocity(bodyId).x;
    auto force = horizontalDampingController.update(error, dt);
    b2Body_ApplyForceToCenter(bodyId, b2MulSV(force, {1, 0}), true);
  }
}

void DemoCreature::jump() { jumpContext.jump = true; }

void DemoCreature::updateJump(float dt) {
  constexpr uint32_t jumpPeriodms = 500;

  auto now = std::chrono::system_clock::now();
  auto duration = now - jumpContext.lastJumpCall;
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  if (ms.count() > jumpPeriodms) {
    // Raycast down to figure out if we should update jumpState
    {
      b2Vec2 castOrigin =
          b2MulSV(0.5f, b2Add(leftLeg->getBasePos(), rightLeg->getBasePos()));
      b2Vec2 castTranslation = b2MulSV(legHeight * 1.2f, {0, -1});
      b2QueryFilter filter = b2DefaultQueryFilter();
      filter.maskBits = filter.maskBits & (~ObjectCategory::CREATURE);
      auto res = PhysicsUtils::getClosestPoint(*world, castOrigin,
                                               castTranslation, filter);
      if (res) {
        jumpContext.jumpState = JumpContext::JumpState::ON_GROUND;
      } else {
        jumpContext.jumpState = JumpContext::JumpState::IN_AIR;
      }
    }

    // Jump
    if (jumpContext.jump) {
      jumpContext.jump = false;
      if (jumpContext.jumpState == JumpContext::JumpState::ON_GROUND) {
        jumpContext.jumpState = JumpContext::JumpState::IN_AIR;
        jumpContext.lastJumpCall = std::chrono::system_clock::now();
        float gravForce = b2Length(b2World_GetGravity(world->getWorldId())) *
                          torso->getPolygon()->getMass();
        b2Vec2 impulse = b2MulSV(gravForce * 3.0, {0, 1});
        b2Body_ApplyLinearImpulseToCenter(torso->getPolygon()->getBodyId(),
                                          impulse, true);
      }
    }
  }
}

void DemoCreature::updateMove(float dt) {
  if (moveContext.move) {
    moveContext.move = false;
    if (jumpContext.jumpState == JumpContext::JumpState::ON_GROUND) {
      auto normDir = b2Normalize(moveContext.dir);
      float desiredHorizontalSpeed =
          moveContext.defaultSpeedMpS *
          std::max(moveContext.maxSpeedMultiplier, moveContext.intensity) *
          normDir.x;
      auto error = desiredHorizontalSpeed -
                   b2Body_GetLinearVelocity(torso->getPolygon()->getBodyId()).x;
      float forceVal = horizontalSpeedController.update(error, dt);
      b2Vec2 force = b2MulSV(forceVal, {1, 0});
      b2Body_ApplyForceToCenter(torso->getPolygon()->getBodyId(), force, true);
    }
  }
}

void DemoCreature::updateFeet(float dt) {
  Direction direction = STANDING;
  constexpr float epsilon = 0.01;
  if (jumpContext.jumpState == JumpContext::JumpState::ON_GROUND) {

    // Find direction of movement
    b2Vec2 dir = b2Normalize(torso->getPolygon()->getLinearVelocity());
    if (dir.x > epsilon) {
      direction = RIGHT;
    } else if (dir.x < -epsilon) {
      direction = LEFT;
    } else {
      direction = STANDING;
    }
  }

  updateLeg(dt, leftFootContext, leftLeg, direction);
  updateLeg(dt, rightFootContext, rightLeg, direction);
}

void DemoCreature::updateLeg(float dt, DemoCreature::FootContext &context,
                             const std::shared_ptr<LimbBody> leg,
                             Direction moveDir) {
  b2Vec2 legBase = leg->getBasePos();
  if (b2Distance(legBase, context.trackingPoint) > leg->getLength() * 1.2f) {
    leg->setTracking({0, 0}, false);
    std::vector<b2Vec2> groundPoints = {};
    b2Vec2 defaultTranslation = {0, -leg->getLength()};
    b2QueryFilter filter = b2DefaultQueryFilter();
    filter.maskBits = filter.maskBits & (~ObjectCategory::CREATURE);
    float stepSize = B2_PI / 36;
    float numOffsets = 10;

    for (int i = -numOffsets; i <= numOffsets; i++) {
      b2Rot rotation = b2MakeRot(stepSize * i);
      b2Vec2 translation = b2RotateVector(rotation, defaultTranslation);
      auto point =
          PhysicsUtils::getClosestPoint((*world), legBase, translation, filter);
      if (point) {
        groundPoints.push_back(*point);
      }
    }
    if (!groundPoints.empty()) {
      b2Vec2 newTrackingPoint = groundPoints[0];
      switch (moveDir) {
      case LEFT:
        for (auto p : groundPoints) {
          if (p.x < newTrackingPoint.x)
            newTrackingPoint = p;
        }
        break;
      case RIGHT:
        for (auto p : groundPoints) {
          if (p.x > newTrackingPoint.x)
            newTrackingPoint = p;
        }
        break;
      case STANDING:
        for (auto p : groundPoints) {
          if (std::abs(p.x - legBase.x) <
              std::abs(newTrackingPoint.x - legBase.x))
            newTrackingPoint = p;
        }
        break;
      }
      leg->setTracking(newTrackingPoint, true);
      context.trackingPoint = newTrackingPoint;
    }
  }
}

void DemoCreature::rotate3D(float angle) { rotate3D(b2MakeRot(angle)); }

void DemoCreature::rotate3D(b2Rot rot) { shoulderConnection->rotate3D(rot); }

void DemoCreature::lookAt(b2Vec2 worldPoint, bool aim) {
  constexpr float planeDist = 15;

  b2Vec2 creaturePos = getWorldPos();

  float translationX = b2Sub(worldPoint, creaturePos).x;

  float desiredAngle = b2Atan2(translationX, planeDist);
  shoulderConnection->rotate3D(desiredAngle);
  hipConnection->rotate3D(desiredAngle);
}

b2Vec2 DemoCreature::getWorldPos() {
  return torso->getPolygon()->getWorldPos();
}