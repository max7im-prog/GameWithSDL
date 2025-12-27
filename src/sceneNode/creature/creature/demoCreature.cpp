#include "demoCreature.hpp"
#include "PIDScalarController.hpp"
#include "box2d/box2d.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include "circleBody.hpp"
#include "creature.hpp"
#include "girdleConnection.hpp"
#include "limbBody.hpp"
#include "miscUtils.hpp"
#include "physicsUtils.hpp"
#include "polygonBody.hpp"
#include "renderComponents.hpp"
#include "world.hpp"
#include <chrono>
#include <nlohmann/json.hpp>
#include <sys/types.h>

void DemoCreature::move(b2Vec2 dir, float intensity) {
  _moveContext._intensity = intensity;
  _moveContext._dir = dir;
  _moveContext._move = true;
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
  float segmentRadius = config.sizeXMeters / 4;
  float torsoWidth = config.sizeXMeters;
  float torsoHeight = config.sizeYMeters;
  auto groupId = ShapeFactory::getNextNegativeId();
  b2Vec2 creaturePos =
      b2Add(config._transform._originPos, config._transform._relativePos);

  b2Vec2 leftShoulderPos =
      b2Add(creaturePos, b2Vec2(-torsoWidth * 0.5, torsoHeight / 2));
  b2Vec2 rightShoulderPos =
      b2Add(creaturePos, b2Vec2(torsoWidth * 0.5, torsoHeight / 2));
  b2Vec2 leftHipPos = b2Add(creaturePos, b2Vec2(-torsoWidth * 0.3, 0));
  b2Vec2 rightHipPos = b2Add(creaturePos, b2Vec2(torsoWidth * 0.3, 0));

  // Create bodies
  PolygonBodyConfig torsoConfig;
  torsoConfig.defaultConfig();
  torsoConfig.shapeCfg.bodyDef.type = b2_dynamicBody;
  torsoConfig.shapeCfg.shapeDef.filter = CreatureConfig::defaultFilter();
  torsoConfig.shapeCfg.shapeDef.filter.groupIndex = groupId;
  torsoConfig.shapeCfg.vertices = {{(-torsoWidth / 4), (0)},
                                   {(-torsoWidth / 4), (torsoHeight)},
                                   {(torsoWidth / 4), (torsoHeight)},
                                   {(torsoWidth / 4), (0)}};

  CircleBodyConfig shoulderConfig;
  shoulderConfig.defaultConfig();
  shoulderConfig.shapeCfg.bodyDef.type = b2_dynamicBody;
  shoulderConfig.shapeCfg.radius = segmentRadius * 1.2f;
  shoulderConfig.shapeCfg.shapeDef.filter = CreatureConfig::defaultFilter();
  shoulderConfig.shapeCfg.shapeDef.filter.groupIndex = groupId;

  LimbBodyConfig limbConfig;
  limbConfig.templateJointConfig.jointDef.dampingRatio = -1000;

  limbConfig.defaultConfig();
  limbConfig.templateCapsuleConfig.bodyDef.type = b2_dynamicBody;
  limbConfig.templateCapsuleConfig.shapeDef.filter =
      CreatureConfig::defaultFilter();
  limbConfig.templateCapsuleConfig.shapeDef.filter.groupIndex = groupId;
  limbConfig.limbControlConfig = {.KPMultiplier = 7.0f,
                                  .KIMultiplier = 0.3f,
                                  .KDMultiplier = 10.0f,
                                  .maxForceMultiplier = 30.0f};

  limbConfig.templateCapsuleConfig.bodyDef.angularDamping = 6;

  limbConfig.rootRot = b2MakeRot(-B2_PI / 2);

  // Limbs
  {
    auto cfg = limbConfig;
    cfg.segments.clear();
    cfg.basePos =
        b2Add(creaturePos, b2Vec2(-torsoWidth * 0.5, torsoHeight / 2));
    {

      auto lastPos = cfg.basePos;
      b2Rot baseRot = b2MakeRot(-B2_PI * 3 / 4);
      b2Rot incrRot = b2MakeRot(0);
      b2Rot q = baseRot;

      for (size_t i = 0; i < numSegments; i++) {
        auto incr = b2MulSV(segmentLen, b2Vec2(1, 0));
        q = b2MulRot(q, incrRot);
        incr = b2RotateVector(q, incr);
        auto newPos = b2Add(incr, lastPos);
        cfg.segments.push_back({.endPos = newPos, .radius = segmentRadius});
        lastPos = newPos;
      }
    }
    // cfg.initialAngleConstraints = {{.minRot = -B2_PI/2,.maxRot =
    // B2_PI/2,.enable = true},{.minRot = -B2_PI/2,.maxRot = B2_PI/2,.enable =
    // true}};
    cfg.initialAngleConstraints = std::vector<AngleConstraint>(numSegments);
    _leftArm = bodyFactory->create<LimbBody>(cfg);
    registerBody(_leftArm, "leftArm");
  }
  {
    auto cfg = limbConfig;
    cfg.segments.clear();
    cfg.basePos = b2Add(creaturePos, b2Vec2(-torsoWidth * 0.3, 0));
    {
      auto lastPos = cfg.basePos;
      b2Rot baseRot = b2MakeRot(-B2_PI * 3 / 4);
      b2Rot incrRot = b2MakeRot(0);
      b2Rot q = baseRot;

      for (size_t i = 0; i < numSegments; i++) {
        auto incr = b2MulSV(segmentLen, b2Vec2(1, 0));
        q = b2MulRot(q, incrRot);
        incr = b2RotateVector(q, incr);
        auto newPos = b2Add(incr, lastPos);
        cfg.segments.push_back({.endPos = newPos, .radius = segmentRadius});
        lastPos = newPos;
      }
    }
    cfg.initialAngleConstraints = std::vector<AngleConstraint>(numSegments);
    _leftLeg = bodyFactory->create<LimbBody>(cfg);
    registerBody(_leftLeg, "leftLeg");
  }
  {
    auto cfg = limbConfig;
    cfg.basePos = b2Add(creaturePos, b2Vec2(torsoWidth * 0.5, torsoHeight / 2));
    {
      auto lastPos = cfg.basePos;
      for (size_t i = 0; i < numSegments; i++) {
        auto incr = b2MulSV(segmentLen, b2Vec2(1, 0));
        b2Rot q = b2MakeRot(-B2_PI / 4);
        incr = b2RotateVector(q, incr);
        auto newPos = b2Add(incr, lastPos);
        cfg.segments.push_back({.endPos = newPos, .radius = segmentRadius});
        lastPos = newPos;
      }
    }
    cfg.initialAngleConstraints = std::vector<AngleConstraint>(numSegments);
    _rightArm = bodyFactory->create<LimbBody>(cfg);
    registerBody(_rightArm, "rightArm");
  }
  {
    auto cfg = limbConfig;
    cfg.basePos = b2Add(creaturePos, b2Vec2(torsoWidth * 0.3, 0));
    {
      auto lastPos = cfg.basePos;
      for (size_t i = 0; i < numSegments; i++) {
        auto incr = b2MulSV(segmentLen, b2Vec2(1, 0));
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
    _rightLeg = bodyFactory->create<LimbBody>(cfg);
    registerBody(_rightLeg, "rightLeg");
  }

  // Torso
  {
    auto cfg = torsoConfig;
    cfg.shapeCfg.bodyDef.position = {creaturePos};
    _torso = bodyFactory->create<PolygonBody>(cfg);
    registerBody(_torso, "torso");
  }

  // Shoulders and hips
  {
    auto cfg = shoulderConfig;
    cfg.shapeCfg.bodyDef.position = leftShoulderPos;
    _leftShoulder = bodyFactory->create<CircleBody>(cfg);
    registerBody(_leftShoulder, "leftShoulder");
  }
  {
    auto cfg = shoulderConfig;
    cfg.shapeCfg.bodyDef.position = rightShoulderPos;
    _rightShoulder = bodyFactory->create<CircleBody>(cfg);
    registerBody(_rightShoulder, "rightShoulder");
  }
  {
    auto cfg = shoulderConfig;
    cfg.shapeCfg.bodyDef.position = leftHipPos;
    _leftHip = bodyFactory->create<CircleBody>(cfg);
    registerBody(_leftHip, "leftHip");
  }
  {
    auto cfg = shoulderConfig;
    cfg.shapeCfg.bodyDef.position = rightHipPos;
    _rightHip = bodyFactory->create<CircleBody>(cfg);
    registerBody(_rightHip, "rightHip");
  }

  // Create connections

  auto bodyPartLocks =
      MiscUtils::lockAll(_leftArm, _leftShoulder, _rightArm, _rightShoulder,
                         _leftLeg, _leftHip, _rightLeg, _rightHip, _torso);

  if (!bodyPartLocks)
    throw std::runtime_error("one or more part expired");
  auto &[leftArmLock, leftShoulderLock, rightArmLock, rightShoulderLock,
         leftLegLock, leftHipLock, rightLegLock, rightHipLock, torsoLock] =
      *bodyPartLocks;
  // Hips
  {
    GirdleConnectionConfig cfg;
    cfg.defaultConfig();
    cfg.girdleWidth = torsoWidth * 0.4;
    cfg.centerAttach.shape = torsoLock->getPolygon();
    cfg.centerAttach.localPoint = {0, 0};

    cfg.leftAttach.shape = leftHipLock->getCircle();
    cfg.leftAttach.localPoint = {0, 0};

    cfg.rightAttach.shape = rightHipLock->getCircle();
    cfg.rightAttach.localPoint = {0, 0};

    cfg.rotationControlTemplate.kp = 1.0f;
    cfg.rotationControlTemplate.kd = 0.0f;
    cfg.rotationControlTemplate.ki = 0.0f;

    cfg.rotationAxis = {0, 1};
    cfg.prismTemplate.jointDef.hertz = 10;

    _hipConnection = connectionFactory->create<GirdleConnection>(cfg);
    registerConnection(_hipConnection, "hipConnection");
  }

  leftLegLock->connect(connectionFactory, leftHipLock->getCircle(), {0, 0});
  rightLegLock->connect(connectionFactory, rightHipLock->getCircle(), {0, 0});

  // Shoulders
  {
    GirdleConnectionConfig cfg;
    cfg.defaultConfig();
    cfg.girdleWidth = torsoWidth * 1.6f;
    cfg.centerAttach.shape = torsoLock->getPolygon();
    cfg.centerAttach.localPoint = {0, torsoHeight / 2.0f};

    cfg.leftAttach.shape = leftShoulderLock->getCircle();
    cfg.leftAttach.localPoint = {0, 0};

    cfg.rightAttach.shape = rightShoulderLock->getCircle();
    cfg.rightAttach.localPoint = {0, 0};

    cfg.rotationControlTemplate.kp = 1.0f;
    cfg.rotationControlTemplate.kd = 0.0f;
    cfg.rotationControlTemplate.ki = 0.0f;

    cfg.rotationAxis = {0, 1};
    cfg.prismTemplate.jointDef.hertz = 10;

    _shoulderConnection = connectionFactory->create<GirdleConnection>(cfg);
    registerConnection(_shoulderConnection, "shoulderConnection");
  }

  leftArmLock->connect(connectionFactory, leftShoulderLock->getCircle(),
                       {0, 0});
  rightArmLock->connect(connectionFactory, rightShoulderLock->getCircle(),
                        {0, 0});

  // Configure controllers
  {
    float inertia = torsoLock->getPolygon()->getRotationalInertia();
    PIDScalarControllerConfig cfg = {.kp = inertia * 400.0f,
                                     .ki = 0.0f,
                                     .kd = inertia * 49.0f,
                                     .maxForce = inertia * 1000};
    _torsoAngleController = PIDScalarController(cfg);
  }
  {

    float force = torsoLock->getPolygon()->getMass() *
                  b2Length(b2World_GetGravity(world->getWorldId()));
    PIDScalarControllerConfig cfg = {.kp = force * 10.0f,
                                     .ki = force * 0.5f,
                                     .kd = force * 16.0f,
                                     .maxForce = force * 15};
    _leftLegHeightController = PIDScalarController(cfg);
    _rightLegHeightController = PIDScalarController(cfg);
  }
  {
    float force = torsoLock->getPolygon()->getMass() *
                  b2Length(b2World_GetGravity(world->getWorldId()));
    PIDScalarControllerConfig cfg = {.kp = force * 5.0f,
                                     .ki = 0.0f,
                                     .kd = force * 16.0f,
                                     .maxForce = force * 3};
    _horizontalDampingController = PIDScalarController(cfg);
  }
  {
    float force = torsoLock->getPolygon()->getMass() *
                  b2Length(b2World_GetGravity(world->getWorldId()));
    PIDScalarControllerConfig cfg = {.kp = force * 5.0f,
                                     .ki = 0.0f,
                                     .kd = force * 16.0f,
                                     .maxForce = force * 10};
    _horizontalSpeedController = PIDScalarController(cfg);
  }

  // Assign values
  _legHeight = segmentLen * numSegments + segmentRadius;
  creatureState = CreatureState::ON_GROUND;
  creatureAbilities = CreatureAbilities::CAN_JUMP;
  _moveContext._maxSpeedMultiplier = 3;
  _moveContext._defaultSpeedMpS = 5;
}

void DemoCreatureConfig::defaultConfig() {
  sizeXMeters = 1;
  sizeYMeters = 1;
}

void DemoCreatureConfig::fromJSON(const nlohmann::json &json) {
  defaultConfig();
  sizeXMeters = json.at("sizeX");
  sizeYMeters = json.at("sizeY");

  if (json.contains("renderConfig")) {
    _renderConfig = SceneNodeConfig::parseRenderConfig(json["renderConfig"]);
  } else {
    _renderConfig = nullptr;
  }
}

void DemoCreature::aim(b2Vec2 worldPoint, bool aim) {
  auto locks = MiscUtils::lockAll(_leftArm, _rightArm);
  if (!locks)
    throw std::runtime_error("One or more elements expired");
  auto &[leftArmLock, rightArmLock] = *locks;
  leftArmLock->setTracking(worldPoint, aim);
  rightArmLock->setTracking(worldPoint, aim);
}

void DemoCreature::update(float dt) {
  keepTorsoUpright(dt);
  keepTorsoAboveTheGround(dt);
  dampHorizontalMovement(dt);
  updateJump(dt);
  updateMove(dt);
  updateFeet(dt);
  updateLookAt(dt);
  updateRotation(dt);
  Creature::update(dt);
}

void DemoCreature::keepTorsoUpright(float dt) {

  auto torsoLock = _torso.lock();
  if (!torsoLock)
    throw std::runtime_error("Torso is expired");
  float error = -b2Rot_GetAngle(torsoLock->getPolygon()->getRotation());
  float torque = _torsoAngleController.update(error, dt);
  torsoLock->getPolygon()->applyTorque(torque);
}

void DemoCreature::keepTorsoAboveTheGround(float dt) {
  b2Vec2 translation = {0, -_legHeight};
  b2QueryFilter filter = b2DefaultQueryFilter();
  filter.maskBits = filter.maskBits & (~ObjectCategory::CREATURE);

  auto locks = MiscUtils::lockAll(_leftLeg, _rightLeg, _torso);
  if (!locks)
    throw std::runtime_error("One or more elements expired");
  auto &[leftLegLock, rightLegLock, torsoLock] = *locks;

  // Left leg
  {
    b2Vec2 castOrigin = leftLegLock->getBasePos();
    auto res = b2World_CastRayClosest(world->getWorldId(), castOrigin,
                                      {0, -_legHeight * 3}, filter);
    if (res.hit) {
      float realHeight = b2Length(b2Sub(res.point, castOrigin));
      if (realHeight < _legHeight * 1.2) {
        float error = _legHeight - realHeight;
        float force = _leftLegHeightController.update(error, dt);
        b2Body_ApplyForce(torsoLock->getPolygon()->getBodyId(), {0, force},
                          castOrigin, true);
      }
    } else {
      _leftLegHeightController.reset();
    }
  }

  // Right leg
  {
    b2Vec2 castOrigin = rightLegLock->getBasePos();
    auto res = b2World_CastRayClosest(world->getWorldId(), castOrigin,
                                      {0, -_legHeight * 3}, filter);
    if (res.hit) {
      float realHeight = b2Length(b2Sub(res.point, castOrigin));
      if (realHeight < _legHeight * 1.2) {
        float error = _legHeight - realHeight;
        float force = _rightLegHeightController.update(error, dt);
        b2Body_ApplyForce(torsoLock->getPolygon()->getBodyId(), {0, force},
                          castOrigin, true);
      }
    } else {
      _rightLegHeightController.reset();
    }
  }
}

void DemoCreature::dampHorizontalMovement(float dt) {
  if (_jumpContext._jumpState == JumpContext::JumpState::ON_GROUND) {

    auto torsoLock = _torso.lock();
    if (!torsoLock)
      throw std::runtime_error("Torso expired");
    auto bodyId = torsoLock->getPolygon()->getBodyId();
    auto error = 0 - b2Body_GetLinearVelocity(bodyId).x;
    auto force = _horizontalDampingController.update(error, dt);
    b2Body_ApplyForceToCenter(bodyId, b2MulSV(force, {1, 0}), true);
  }
}

void DemoCreature::jump() { _jumpContext._jump = true; }

void DemoCreature::updateJump(float dt) {
  constexpr uint32_t jumpPeriodms = 500;

  auto locks = MiscUtils::lockAll(_leftLeg, _rightLeg, _torso);
  if (!locks)
    throw std::runtime_error("One or more elements expired");
  auto &[leftLegLock, rightLegLock, torsoLock] = *locks;

  auto now = std::chrono::system_clock::now();
  auto duration = now - _jumpContext._lastJumpCall;
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  if (ms.count() > jumpPeriodms) {
    // Raycast down to figure out if we should update jumpState
    {
      b2Vec2 castOrigin = b2MulSV(
          0.5f, b2Add(leftLegLock->getBasePos(), rightLegLock->getBasePos()));
      b2Vec2 castTranslation = b2MulSV(_legHeight * 1.2f, {0, -1});
      b2QueryFilter filter = b2DefaultQueryFilter();
      filter.maskBits = filter.maskBits & (~ObjectCategory::CREATURE);
      auto res = PhysicsUtils::getClosestPoint(*world, castOrigin,
                                               castTranslation, filter);
      if (res) {
        _jumpContext._jumpState = JumpContext::JumpState::ON_GROUND;
      } else {
        _jumpContext._jumpState = JumpContext::JumpState::IN_AIR;
      }
    }

    // Jump
    if (_jumpContext._jump) {
      _jumpContext._jump = false;
      if (_jumpContext._jumpState == JumpContext::JumpState::ON_GROUND) {
        _jumpContext._jumpState = JumpContext::JumpState::IN_AIR;
        _jumpContext._lastJumpCall = std::chrono::system_clock::now();
        float gravForce = b2Length(b2World_GetGravity(world->getWorldId())) *
                          torsoLock->getPolygon()->getMass();
        b2Vec2 impulse = b2MulSV(gravForce * 10.0, {0, 1});
        b2Body_ApplyLinearImpulseToCenter(torsoLock->getPolygon()->getBodyId(),
                                          impulse, true);
      }
    }
  }
}

void DemoCreature::updateMove(float dt) {
  if (_moveContext._move) {
    _moveContext._move = false;
    if (_jumpContext._jumpState == JumpContext::JumpState::ON_GROUND) {
      auto locks = MiscUtils::lockAll(_torso);
      if (!locks)
        throw std::runtime_error("One or more elements expired");
      auto &[torsoLock] = *locks;
      auto normDir = b2Normalize(_moveContext._dir);
      float desiredHorizontalSpeed =
          _moveContext._defaultSpeedMpS *
          std::max(_moveContext._maxSpeedMultiplier, _moveContext._intensity) *
          normDir.x;
      auto error =
          desiredHorizontalSpeed -
          b2Body_GetLinearVelocity(torsoLock->getPolygon()->getBodyId()).x;
      float forceVal = _horizontalSpeedController.update(error, dt);
      b2Vec2 force = b2MulSV(forceVal, {1, 0});
      b2Body_ApplyForceToCenter(torsoLock->getPolygon()->getBodyId(), force,
                                true);
    }
  }
}

void DemoCreature::updateFeet(float dt) {
  Direction direction = STANDING;
  constexpr float epsilon = 0.01;

  auto locks = MiscUtils::lockAll(_torso, _leftLeg, _rightLeg);
  if (!locks)
    throw std::runtime_error("One or more elements expired");
  auto &[torsoLock, leftLegLock, rightLegLock] = *locks;
  if (_jumpContext._jumpState == JumpContext::JumpState::ON_GROUND) {

    // Find direction of movement
    b2Vec2 dir = b2Normalize(torsoLock->getPolygon()->getLinearVelocity());
    if (dir.x > epsilon) {
      direction = RIGHT;
    } else if (dir.x < -epsilon) {
      direction = LEFT;
    } else {
      direction = STANDING;
    }
  }

  updateLeg(dt, _leftFootContext, leftLegLock, direction);
  updateLeg(dt, _rightFootContext, rightLegLock, direction);
}

void DemoCreature::updateLeg(float dt, DemoCreature::FootContext &context,
                             const std::shared_ptr<LimbBody> leg,
                             Direction moveDir) {
  b2Vec2 legBase = leg->getBasePos();
  if (b2Distance(legBase, context.trackingPoint) > leg->getLength() * 1.2f) {
    leg->setTracking({0, 0}, false);
    std::vector<b2Vec2> groundPoints = {};
    b2Vec2 defaultTranslation = {0, -leg->getLength() * 1.5f};
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

void DemoCreature::lookAt(b2Vec2 worldPoint, bool aim) {
  constexpr float planeDist = 15;
  auto locks = MiscUtils::lockAll(_shoulderConnection, _hipConnection);
  if (!locks)
    throw std::runtime_error("One or more elements expired");
  auto &[shoulderConnectionLock, hipConnectionLock] = *locks;

  b2Vec2 creaturePos = getWorldPos();

  float translationX = b2Sub(worldPoint, creaturePos).x;

  float desiredAngle = b2Atan2(translationX, planeDist);
  shoulderConnectionLock->rotate3D(desiredAngle);
  hipConnectionLock->rotate3D(desiredAngle);

  _registry.emplace_or_replace<RenderRequiresUpdateTag>(getEntity());
}

void DemoCreature::updateLookAt(float dt) {}
void DemoCreature::updateRotation(float dt) {}

b2Vec2 DemoCreature::getWorldPos() {

  auto torsoLock = _torso.lock();
  if (!torsoLock)
    throw std::runtime_error("Torso expired");
  return torsoLock->getPolygon()->getWorldPos();
}

const b2Rot &DemoCreature::getRotation() const {
  return _rotationContext._rotation;


}
