#include "human.hpp"
#include "bodyFactory.hpp"
#include "box2d/box2d.h"
#include "box2d/collision.h"
#include "box2d/types.h"
#include "girdleConnection.hpp"
#include "kinematicUtils.hpp"
#include "limbBody.hpp"
#include "revoluteConnection.hpp"
#include "shapeFactory.hpp"
#include <stdexcept>

Human::Human(entt::registry &registry, const std::shared_ptr<World> world,
             const Human::Config &config,
             const std::shared_ptr<BodyFactory> bodyFactory,
             const std::shared_ptr<ConnectionFactory> connectionFactory)
    : Creature(registry, world) {
  auto initInfo = computeInitInfo(config);
  auto anatomyTemplates = createAnatomyTemplates(config, initInfo);
  createAnatomy(config, initInfo, anatomyTemplates, bodyFactory,
                connectionFactory);
  bindBehavior();
  bindActions();
}

Human::InitInfo Human::computeInitInfo(const Human::Config &config) {
  Human::InitInfo ret;

  // Calculate body positions
  ret._initDimensions._basePos =
      b2Add(config._transform._originPos, config._transform._relativePos);
  float baseSize = config._proportions._baseSizeMeters;

  ret._initDimensions._leftShoulderPos =
      b2Add(ret._initDimensions._basePos,
            {baseSize * config._proportions._shoulderGirdleWIdhtRatio / 2.0f,
             baseSize * config._proportions._torsoHeightRatio / 2.0f});
  ret._initDimensions._rightShoulderPos =
      b2Add(ret._initDimensions._basePos,
            {-baseSize * config._proportions._shoulderGirdleWIdhtRatio / 2.0f,
             baseSize * config._proportions._torsoHeightRatio / 2.0f});
  ret._initDimensions._leftHipPos =
      b2Add(ret._initDimensions._basePos,
            {baseSize * config._proportions._shoulderGirdleWIdhtRatio / 2.0f,
             -baseSize * config._proportions._torsoHeightRatio / 2.0f});
  ret._initDimensions._rightHipPos =
      b2Add(ret._initDimensions._basePos,
            {-baseSize * config._proportions._shoulderGirdleWIdhtRatio / 2.0f,
             -baseSize * config._proportions._torsoHeightRatio / 2.0f});
  ret._initDimensions._neckPos =
      b2Add(ret._initDimensions._basePos,
            {0, baseSize *
                    (config._proportions._torsoHeightRatio +
                     config._proportions._neckRatio) /
                    2.0f});
  ret._initDimensions._headPos =
      b2Add(ret._initDimensions._basePos,
            {0, baseSize * (config._proportions._torsoHeightRatio / 2.0f +
                            config._proportions._headHeightRatio / 2.0f +
                            config._proportions._neckRatio)});

  // Compute densities
  auto &prop = config._proportions;
  auto &mass = config._massDistribution;

  ret._densities._head = (mass._totalMassKg * mass._headRatio) /
                         (prop._baseSizeMeters * prop._headHeightRatio *
                          prop._baseSizeMeters * prop._headWidthRatio);
  ret._densities._neck = (mass._totalMassKg * mass._neckRatio) /
                         (prop._baseSizeMeters * prop._neckRatio *
                          prop._baseSizeMeters * prop._limbThicknessRatio);
  ret._densities._torso = (mass._totalMassKg * mass._torsoRatio) /
                          (prop._baseSizeMeters * prop._torsoHeightRatio *
                           prop._baseSizeMeters * prop._torsoWidthRatio);
  ret._densities._shoulder = (mass._totalMassKg * mass._shoulderRatio / 2.0f) /
                             ((prop._baseSizeMeters * prop._shoulderSizeRatio *
                               prop._baseSizeMeters * prop._shoulderSizeRatio) *
                              B2_PI / 4);
  ret._densities._hip = (mass._totalMassKg * mass._hipRatio / 2.0f) /
                        ((prop._baseSizeMeters * prop._hipSizeRatio *
                          prop._baseSizeMeters * prop._hipSizeRatio) *
                         B2_PI / 4);
  ret._densities._leftArm = (mass._totalMassKg * mass._armRatio / 2.0f) /
                            (prop._baseSizeMeters * prop._leftArmRatio *
                             prop._baseSizeMeters * prop._limbThicknessRatio);
  ret._densities._rightArm = (mass._totalMassKg * mass._armRatio / 2.0f) /
                             (prop._baseSizeMeters * prop._rightArmRatio *
                              prop._baseSizeMeters * prop._limbThicknessRatio);
  ret._densities._leftLeg = (mass._totalMassKg * mass._legRatio / 2.0f) /
                            (prop._baseSizeMeters * prop._leftLegRatio *
                             prop._baseSizeMeters * prop._limbThicknessRatio);
  ret._densities._rightLeg = (mass._totalMassKg * mass._legRatio / 2.0f) /
                             (prop._baseSizeMeters * prop._rightLegRatio *
                              prop._baseSizeMeters * prop._limbThicknessRatio);

  // Assign a group ID
  //
  // All body parts of a human should not collide,
  // therefore group ID applied to all bodies is negative
  ret._groupId = ShapeFactory::getNextNegativeId();

  return ret;
}

Human::InitAnatomyTemplates
Human::createAnatomyTemplates(const Human::Config &config,
                              const Human::InitInfo &initInfo) {
  Human::InitAnatomyTemplates templates;

  // Torso
  templates._torsoTemplate.defaultConfig();
  templates._torsoTemplate.shapeCfg.bodyDef.type = b2_dynamicBody;
  templates._torsoTemplate.shapeCfg.shapeDef.filter.groupIndex =
      initInfo._groupId;

  // Neck
  templates._neckTemplate.defaultConfig();
  templates._neckTemplate.shapeCfg.bodyDef.type = b2_dynamicBody;
  templates._neckTemplate.shapeCfg.shapeDef.filter.groupIndex =
      initInfo._groupId;

  // Head
  templates._headTemplate.defaultConfig();
  templates._headTemplate.shapeCfg.bodyDef.type = b2_dynamicBody;
  templates._headTemplate.shapeCfg.shapeDef.filter.groupIndex =
      initInfo._groupId;

  // Shoulders
  templates._shoulderTemplate.defaultConfig();
  templates._shoulderTemplate.shapeCfg.bodyDef.type = b2_dynamicBody;
  templates._shoulderTemplate.shapeCfg.shapeDef.filter.groupIndex =
      initInfo._groupId;

  // Hips
  templates._hipTemplate.defaultConfig();
  templates._hipTemplate.shapeCfg.bodyDef.type = b2_dynamicBody;
  templates._hipTemplate.shapeCfg.shapeDef.filter.groupIndex =
      initInfo._groupId;

  // Limbs
  templates._limbTemplate.defaultConfig();
  templates._limbTemplate.templateCapsuleConfig.bodyDef.type = b2_dynamicBody;
  templates._limbTemplate.templateCapsuleConfig.shapeDef.filter.groupIndex =
      initInfo._groupId;
  templates._limbTemplate.rootRot = b2MakeRot(-B2_PI / 2);

  // Hip Girdle
  templates._hipGirdleTemplate.defaultConfig();

  // Shoulder Girdle
  templates._shoulderGirdleTemplate.defaultConfig();

  // Head to neck
  templates._headToNeckTemplate.defaultConfig();

  // neck to torso
  templates._neckToTorsoTemplate.defaultConfig();

  return templates;
}

void Human::createAnatomy(
    const Human::Config &config, const InitInfo &initInfo,
    const InitAnatomyTemplates &initAnatomyTemplates,
    const std::shared_ptr<BodyFactory> &bodyFactory,
    const std::shared_ptr<ConnectionFactory> &connectionFactory) {
  // Torso
  {
    PolygonBody::Config cfg = initAnatomyTemplates._torsoTemplate;
    cfg.shapeCfg.bodyDef.position = initInfo._initDimensions._basePos;
    cfg.shapeCfg.shapeDef.density = initInfo._densities._torso;
    cfg.shapeCfg.vertices = {{(config._proportions._baseSizeMeters *
                               -config._proportions._torsoWidthRatio / 2.0f),
                              (config._proportions._baseSizeMeters *
                               -config._proportions._torsoHeightRatio / 2.0f)},

                             {(config._proportions._baseSizeMeters *
                               -config._proportions._torsoWidthRatio / 2.0f),
                              (config._proportions._baseSizeMeters *
                               config._proportions._torsoHeightRatio / 2.0f)},

                             {(config._proportions._baseSizeMeters *
                               config._proportions._torsoWidthRatio / 2.0f),
                              (config._proportions._baseSizeMeters *
                               config._proportions._torsoHeightRatio / 2.0f)},

                             {(config._proportions._baseSizeMeters *
                               config._proportions._torsoWidthRatio / 2.0f),
                              (config._proportions._baseSizeMeters *
                               -config._proportions._torsoHeightRatio / 2.0f)}};
    _bodies._torso = bodyFactory->create<PolygonBody>(cfg);
    registerBody(_bodies._torso, BodyNames::s_torso);
  }

  // Neck
  {
    CapsuleBody::Config cfg = initAnatomyTemplates._neckTemplate;
    cfg.shapeCfg.bodyDef.position = initInfo._initDimensions._neckPos;
    cfg.shapeCfg.shapeDef.density = initInfo._densities._neck;
    cfg.shapeCfg.center1 = {0, -config._proportions._baseSizeMeters *
                                   config._proportions._neckRatio / 2.0f};
    cfg.shapeCfg.center2 = {0, config._proportions._baseSizeMeters *
                                   config._proportions._neckRatio / 2.0f};
    cfg.shapeCfg.radius = config._proportions._baseSizeMeters *
                          config._proportions._limbThicknessRatio / 2.0f;
    _bodies._neck = bodyFactory->create<CapsuleBody>(cfg);
    registerBody(_bodies._neck, BodyNames::s_neck);
  }

  // Head
  {
    PolygonBody::Config cfg = initAnatomyTemplates._torsoTemplate;
    cfg.shapeCfg.bodyDef.position = initInfo._initDimensions._headPos;
    cfg.shapeCfg.shapeDef.density = initInfo._densities._head;
    cfg.shapeCfg.vertices = {{(config._proportions._baseSizeMeters *
                               -config._proportions._headWidthRatio / 2.0f),
                              (config._proportions._baseSizeMeters *
                               -config._proportions._headHeightRatio / 2.0f)},

                             {(config._proportions._baseSizeMeters *
                               -config._proportions._headWidthRatio / 2.0f),
                              (config._proportions._baseSizeMeters *
                               config._proportions._headHeightRatio / 2.0f)},

                             {(config._proportions._baseSizeMeters *
                               config._proportions._headWidthRatio / 2.0f),
                              (config._proportions._baseSizeMeters *
                               config._proportions._headHeightRatio / 2.0f)},

                             {(config._proportions._baseSizeMeters *
                               config._proportions._headWidthRatio / 2.0f),
                              (config._proportions._baseSizeMeters *
                               -config._proportions._headHeightRatio / 2.0f)}};
    _bodies._head = bodyFactory->create<PolygonBody>(cfg);
    registerBody(_bodies._head, BodyNames::s_head);
  }

  // Left Shoulder
  {
    CircleBody::Config cfg = initAnatomyTemplates._shoulderTemplate;
    cfg.shapeCfg.bodyDef.position = initInfo._initDimensions._leftShoulderPos;
    cfg.shapeCfg.shapeDef.density = initInfo._densities._shoulder;
    cfg.shapeCfg.radius = config._proportions._baseSizeMeters *
                          config._proportions._shoulderSizeRatio / 2.0f;
    _bodies._shoulderLeft = bodyFactory->create<CircleBody>(cfg);
    registerBody(_bodies._shoulderLeft, BodyNames::s_leftShoulder);
  }

  // Right Shoulder
  {
    CircleBody::Config cfg = initAnatomyTemplates._shoulderTemplate;
    cfg.shapeCfg.bodyDef.position = initInfo._initDimensions._rightShoulderPos;
    cfg.shapeCfg.shapeDef.density = initInfo._densities._shoulder;
    cfg.shapeCfg.radius = config._proportions._baseSizeMeters *
                          config._proportions._shoulderSizeRatio / 2.0f;
    _bodies._shoulderRight = bodyFactory->create<CircleBody>(cfg);
    registerBody(_bodies._shoulderRight, BodyNames::s_rightShoulder);
  }

  // Left Hip
  {
    CircleBody::Config cfg = initAnatomyTemplates._hipTemplate;
    cfg.shapeCfg.bodyDef.position = initInfo._initDimensions._leftHipPos;
    cfg.shapeCfg.shapeDef.density = initInfo._densities._hip;
    cfg.shapeCfg.radius = config._proportions._baseSizeMeters *
                          config._proportions._hipSizeRatio / 2.0f;
    _bodies._hipLeft = bodyFactory->create<CircleBody>(cfg);
    registerBody(_bodies._hipLeft, BodyNames::s_leftHip);
  }

  // Right Hip
  {
    CircleBody::Config cfg = initAnatomyTemplates._hipTemplate;
    cfg.shapeCfg.bodyDef.position = initInfo._initDimensions._rightHipPos;
    cfg.shapeCfg.shapeDef.density = initInfo._densities._hip;
    cfg.shapeCfg.radius = config._proportions._baseSizeMeters *
                          config._proportions._hipSizeRatio / 2.0f;
    _bodies._hipRight = bodyFactory->create<CircleBody>(cfg);
    registerBody(_bodies._hipRight, BodyNames::s_rightHip);
  }

  // Limbs
  constexpr int segmentsPerLimb = 2;

  // Left arm
  {
    LimbBody::Config cfg = initAnatomyTemplates._limbTemplate;
    cfg.basePos = initInfo._initDimensions._leftShoulderPos;
    cfg.templateCapsuleConfig.shapeDef.density = initInfo._densities._leftArm;
    cfg.rootRot = b2MakeRot(0);
    cfg.segments.clear();
    cfg.initialAngleConstraints = std::vector<AngleConstraint>(segmentsPerLimb);
    b2Vec2 lastPos{0, 0};
    b2Vec2 incr{b2MulSV(config._proportions._baseSizeMeters *
                            config._proportions._leftArmRatio / segmentsPerLimb,
                        {1, 0})};
    for (int i{0}; i < segmentsPerLimb; ++i) {
      LimbBody::Config::SegmentConfig seg;
      seg.radius = config._proportions._limbThicknessRatio *
                   config._proportions._baseSizeMeters / 2.0f;
      lastPos = b2Add(lastPos, incr);
      seg.endPos = lastPos;
      cfg.segments.push_back(seg);
    }
    _bodies._armLeft = bodyFactory->create<LimbBody>(cfg);
    registerBody(_bodies._armLeft, BodyNames::s_leftArm);
  }

  // Right arm
  {
    LimbBody::Config cfg = initAnatomyTemplates._limbTemplate;
    cfg.basePos = initInfo._initDimensions._rightShoulderPos;
    cfg.templateCapsuleConfig.shapeDef.density = initInfo._densities._rightArm;
    cfg.rootRot = b2MakeRot(B2_PI);
    cfg.segments.clear();
    cfg.initialAngleConstraints = std::vector<AngleConstraint>(segmentsPerLimb);
    b2Vec2 lastPos{0, 0};
    b2Vec2 incr{b2MulSV(config._proportions._baseSizeMeters *
                            config._proportions._rightArmRatio /
                            segmentsPerLimb,
                        {1, 0})};
    for (int i{0}; i < segmentsPerLimb; ++i) {
      LimbBody::Config::SegmentConfig seg;
      seg.radius = config._proportions._limbThicknessRatio *
                   config._proportions._baseSizeMeters / 2.0f;
      lastPos = b2Add(lastPos, incr);
      seg.endPos = lastPos;
      cfg.segments.push_back(seg);
    }
    _bodies._armRight = bodyFactory->create<LimbBody>(cfg);
    registerBody(_bodies._armRight, BodyNames::s_rightArm);
  }

  // Left leg
  {
    LimbBody::Config cfg = initAnatomyTemplates._limbTemplate;
    cfg.basePos = initInfo._initDimensions._leftHipPos;
    cfg.templateCapsuleConfig.shapeDef.density = initInfo._densities._leftLeg;
    cfg.rootRot = b2MakeRot(-B2_PI / 2);
    cfg.segments.clear();
    cfg.initialAngleConstraints = std::vector<AngleConstraint>(segmentsPerLimb);
    b2Vec2 lastPos{0, 0};
    b2Vec2 incr{b2MulSV(config._proportions._baseSizeMeters *
                            config._proportions._leftLegRatio / segmentsPerLimb,
                        {1, 0})};
    for (int i{0}; i < segmentsPerLimb; ++i) {
      LimbBody::Config::SegmentConfig seg;
      seg.radius = config._proportions._limbThicknessRatio *
                   config._proportions._baseSizeMeters / 2.0f;
      lastPos = b2Add(lastPos, incr);
      seg.endPos = lastPos;
      cfg.segments.push_back(seg);
    }
    _bodies._legLeft = bodyFactory->create<LimbBody>(cfg);
    registerBody(_bodies._legLeft, BodyNames::s_leftLeg);
  }

  // Right leg
  {
    LimbBody::Config cfg = initAnatomyTemplates._limbTemplate;
    cfg.basePos = initInfo._initDimensions._rightHipPos;
    cfg.templateCapsuleConfig.shapeDef.density = initInfo._densities._rightLeg;
    cfg.rootRot = b2MakeRot(-B2_PI / 2);
    cfg.segments.clear();
    cfg.initialAngleConstraints = std::vector<AngleConstraint>(segmentsPerLimb);
    b2Vec2 lastPos{0, 0};
    b2Vec2 incr{b2MulSV(config._proportions._baseSizeMeters *
                            config._proportions._rightLegRatio /
                            segmentsPerLimb,
                        {1, 0})};
    for (int i{0}; i < segmentsPerLimb; ++i) {
      LimbBody::Config::SegmentConfig seg;
      seg.radius = config._proportions._limbThicknessRatio *
                   config._proportions._baseSizeMeters / 2.0f;
      lastPos = b2Add(lastPos, incr);
      seg.endPos = lastPos;
      cfg.segments.push_back(seg);
    }
    _bodies._legRight = bodyFactory->create<LimbBody>(cfg);
    registerBody(_bodies._legRight, BodyNames::s_rightLeg);
  }

  // Shoulder girdle
  {
    GirdleConnection::Config cfg{initAnatomyTemplates._shoulderGirdleTemplate};
    auto leftLock = _bodies._shoulderLeft.lock();
    auto rightLock = _bodies._shoulderRight.lock();
    auto centerLock = _bodies._torso.lock();
    if (leftLock && rightLock && centerLock) {
      cfg._initial3DRotationRad = 0;
      // cfg._centerAttach._shape = centerLock->getPolygon();
      cfg._centerAttach._shapeLocalPoint = {
          0, config._proportions._baseSizeMeters *
                 config._proportions._torsoHeightRatio / 2.0f};
      // cfg._leftAttach._shape = leftLock->getCircle();
      cfg._leftAttach._shapeLocalPoint = {0, 0};
      // cfg._rightAttach._shape = rightLock->getCircle();
      cfg._rightAttach._shapeLocalPoint = {0, 0};
      cfg._girdleWidth = config._proportions._baseSizeMeters *
                         config._proportions._shoulderGirdleWIdhtRatio;
      _connections._shoulders =
          connectionFactory->create<GirdleConnection>(cfg);
      registerConnection(_connections._shoulders,
                         ConnectionNames::s_shoulderGirdle);
    } else {
      throw std::runtime_error(
          "Human: One or more bodies expired on shoulder creation");
    }
  }

  // Hip girdle
  {
    GirdleConnection::Config cfg{initAnatomyTemplates._hipGirdleTemplate};
    auto leftLock = _bodies._hipLeft.lock();
    auto rightLock = _bodies._hipRight.lock();
    auto centerLock = _bodies._torso.lock();
    if (leftLock && rightLock && centerLock) {
      cfg._initial3DRotationRad = 0;
      // cfg._centerAttach._shape = centerLock->getPolygon();
      cfg._centerAttach._shapeLocalPoint = {
          0, -config._proportions._baseSizeMeters *
                 config._proportions._torsoHeightRatio / 2.0f};
      // cfg._leftAttach._shape = leftLock->getCircle();
      cfg._leftAttach._shapeLocalPoint = {0, 0};
      // cfg._rightAttach._shape = rightLock->getCircle();
      cfg._rightAttach._shapeLocalPoint = {0, 0};
      cfg._girdleWidth = config._proportions._baseSizeMeters *
                         config._proportions._hipGirdleWidthRatio;
      _connections._hips = connectionFactory->create<GirdleConnection>(cfg);
      registerConnection(_connections._hips, ConnectionNames::s_hipGirdle);
    } else {
      throw std::runtime_error(
          "Human: One or more bodies expired on shoulder creation");
    }
  }

  // Connect head to neck
  {
    RevoluteConnection::Config cfg{initAnatomyTemplates._headToNeckTemplate};
  }

  // Connect limbs
  {
    auto limbLock = _bodies._armLeft.lock();
    auto attach = _bodies._shoulderLeft;
    if (limbLock) {
      limbLock->connect(connectionFactory, attach, "main", {0, 0}, attach);
    } else {
      throw std::runtime_error(
          "Human: failed to attach limb - one or more bodies expired");
    }
  }
  {
    auto limbLock = _bodies._armRight.lock();
    auto attach = _bodies._shoulderLeft;
    if (limbLock) {
      limbLock->connect(connectionFactory, attach, "main", {0, 0}, attach);
    } else {
      throw std::runtime_error(
          "Human: failed to attach limb - one or more bodies expired");
    }
  }
  {
    auto limbLock = _bodies._legLeft.lock();
    auto attach = _bodies._shoulderLeft;
    if (limbLock) {
      limbLock->connect(connectionFactory, attach, "main", {0, 0}, attach);
    } else {
      throw std::runtime_error(
          "Human: failed to attach limb - one or more bodies expired");
    }
  }
  {
    auto limbLock = _bodies._legRight.lock();
    auto attach = _bodies._shoulderLeft;
    if (limbLock) {
      limbLock->connect(connectionFactory, attach, "main", {0, 0}, attach);
    } else {
      throw std::runtime_error(
          "Human: failed to attach limb - one or more bodies expired");
    }
  }

  // TODO: complete
}

void Human::bindActions() {
  // TODO: complete
}

void Human::bindBehavior() {
  // TODO: complete
}

void Human::Config::defaultConfig() {
  // TODO: implement
}

void Human::Config::fromJSON(const nlohmann::json &json) {
  // TODO: implement
}

void Human::move(b2Vec2 dir) {
  // TODO: implement
}

void Human::lookAt(b2Vec2 worldPoint) {
  // TODO: implement
}

void Human::update(float dt) {
  // TODO: implement
}

b2Vec2 Human::getWorldPos() {
  // TODO: implement
  if (auto l = _bodies._torso.lock()) {
    return l->getWorldPos();
  }
  return {0, 0};
}

b2Rot Human::getRotation() const {
  // TODO: implement
  return {};
}
