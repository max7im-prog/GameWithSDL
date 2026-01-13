#include "human.hpp"
#include "bodyFactory.hpp"
#include "box2d/box2d.h"
#include "box2d/collision.h"
#include "box2d/types.h"
#include "shapeFactory.hpp"

Human::Human(entt::registry &registry, const std::shared_ptr<World> world,
             const HumanConfig &config,
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
            {baseSize * config._proportions._shoulderWidthRatio / 2.0f,
             baseSize * config._proportions._torsoHeightRatio / 2.0f});
  ret._initDimensions._rightShoulderPos =
      b2Add(ret._initDimensions._basePos,
            {-baseSize * config._proportions._shoulderWidthRatio / 2.0f,
             baseSize * config._proportions._torsoHeightRatio / 2.0f});
  ret._initDimensions._leftHipPos =
      b2Add(ret._initDimensions._basePos,
            {baseSize * config._proportions._shoulderWidthRatio / 2.0f,
             -baseSize * config._proportions._torsoHeightRatio / 2.0f});
  ret._initDimensions._rightHipPos =
      b2Add(ret._initDimensions._basePos,
            {-baseSize * config._proportions._shoulderWidthRatio / 2.0f,
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
  // TODO: magic number
  templates._hipGirdleTemplate.defaultConfig();

  // Shoulder Girdle
  templates._shoulderGirdleTemplate.defaultConfig();

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
    registerBody(_bodies._torso, "torso");
  }
  // TODO: complete
}

void Human::bindActions() {
  // TODO: complete
}

void Human::bindBehavior() {
  // TODO: complete
}
