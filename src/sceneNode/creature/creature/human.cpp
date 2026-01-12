#include "human.hpp"
#include "bodyFactory.hpp"
#include "box2d/types.h"
#include "shapeFactory.hpp"

Human::Human(entt::registry &registry, const std::shared_ptr<World> world,
             const HumanConfig &config,
             const std::shared_ptr<BodyFactory> bodyFactory,
             const std::shared_ptr<ConnectionFactory> connectionFactory)
    : Creature(registry, world) {

  // Set actions
  _actions = {}; // Actions are empty for now

  // Calculate body positions
  b2Vec2 basePos =
      b2Add(config._transform._originPos, config._transform._relativePos);
  float baseSize = config._proportions._baseSizeMeters;

  b2Vec2 leftShoulderPos =
      b2Add(basePos, {baseSize * config._proportions._shoulderWidthRatio / 2.0f,
                      baseSize * config._proportions._torsoHeightRatio / 2.0f});
  b2Vec2 rightShoulderPos = b2Add(
      basePos, {-baseSize * config._proportions._shoulderWidthRatio / 2.0f,
                baseSize * config._proportions._torsoHeightRatio / 2.0f});
  b2Vec2 leftHipPos = b2Add(
      basePos, {baseSize * config._proportions._shoulderWidthRatio / 2.0f,
                -baseSize * config._proportions._torsoHeightRatio / 2.0f});
  b2Vec2 rightHipPos = b2Add(
      basePos, {-baseSize * config._proportions._shoulderWidthRatio / 2.0f,
                -baseSize * config._proportions._torsoHeightRatio / 2.0f});
  b2Vec2 neckPos =
      b2Add(basePos, {0, baseSize *
                             (config._proportions._torsoHeightRatio +
                              config._proportions._neckRatio) /
                             2.0f});
  b2Vec2 headPos = b2Add(
      basePos, {0, baseSize * (config._proportions._torsoHeightRatio / 2.0f +
                               config._proportions._headHeightRatio / 2.0f +
                               config._proportions._neckRatio)});

  // All body parts of a human should not collide,
  // therefore group ID applied to all bodies is negative
  int groupId = ShapeFactory::getNextNegativeId();

  // Torso
  {
    PolygonBody::Config cfg;
    cfg.defaultConfig();
    cfg.shapeCfg.bodyDef.type = b2_dynamicBody;
    cfg.shapeCfg.shapeDef.filter = CreatureConfig::defaultFilter();
    cfg.shapeCfg.shapeDef.filter.groupIndex = groupId;
    cfg.shapeCfg.vertices = {
        {(baseSize * -config._proportions._torsoWidthRatio / 2.0f),
         (baseSize * -config._proportions._torsoHeightRatio / 2.0f)},

        {(baseSize * -config._proportions._torsoWidthRatio / 2.0f),
         (baseSize * config._proportions._torsoHeightRatio / 2.0f)},

        {(baseSize * config._proportions._torsoWidthRatio / 2.0f),
         (baseSize * config._proportions._torsoHeightRatio / 2.0f)},

        {(baseSize * config._proportions._torsoWidthRatio / 2.0f),
         (baseSize * -config._proportions._torsoHeightRatio / 2.0f)}};
    _torso = bodyFactory->create<PolygonBody>(cfg);
    registerBody(_torso, "torso");
  }
}
