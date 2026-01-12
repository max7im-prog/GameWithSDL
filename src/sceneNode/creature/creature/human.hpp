
#pragma once
#include "bodyFactory.hpp"
#include "connectionFactory.hpp"
#include "creature.hpp"
#include <entt/entt.hpp>

// Here are the responsibilities of models:
//
// HumanArticulationModel - skeletal constraints (hips,shoulders,angle
// constraints forarms and legs, pseudo 3d rotation, joint limits
//
// HumanLocomotionModel - leg movement logic, jump state, movement state
//
// HumanBalanceModel - torso vertical offset, horizontal damping, etc.
//
// HumaAimModel - where arms point at, head inclination

struct HumanConfig : public CreatureConfig {
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;
  struct {
    float _baseSizeMeters = 1.0f;

    float _shoulderWidthRatio = 0.3f;
    float _shoulderSizeRatio = 0.3f;

    float _hipRatio = 0.3f;
    float _hipSizeRatio = 0.3f;

    float _torsoHeightRatio = 0.4;
    float _torsoWidthRatio = 0.4;

    float _headHeightRatio = 0.1f;
    float _headWidthRatio = 0.1f;

    float _limbThicknessRatio = 0.1f;
    float _neckRatio = 0.1f;
    float _leftArmRatio = 0.4f;
    float _rightArmRatio = 0.4f;
    float _leftLegRatio = 0.4f;
    float _rightLegRatio = 0.4f;
  } _proportions;
};

class Human : public Creature, public VisitableImpl<Human> {
public:
  using Config = HumanConfig;

  void move(b2Vec2 dir) override;
  void lookAt(b2Vec2 worldPoint) override;
  virtual void update(float dt) override;
  virtual ~Human() = default;

  b2Vec2 getWorldPos() override;

  const b2Rot getRotation() const;

protected:
  Human(entt::registry &registry, const std::shared_ptr<World> world,
        const HumanConfig &config,
        const std::shared_ptr<BodyFactory> bodyFactory,
        const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::weak_ptr<PolygonBody> _head;
  std::weak_ptr<CapsuleBody> _neck;
  std::weak_ptr<PolygonBody> _torso;
  std::weak_ptr<CircleBody> _shoulderLeft;
  std::weak_ptr<CircleBody> _shoulderRight;
  std::weak_ptr<CircleBody> _hipLeft;
  std::weak_ptr<CircleBody> _hipRight;
  std::weak_ptr<LimbBody> _armLeft;
  std::weak_ptr<LimbBody> _armRight;
  std::weak_ptr<LimbBody> _legLeft;
  std::weak_ptr<LimbBody> _legRight;

private:
  // A struct used during initialization of a human, not used anywhere else
  struct InitInfo {
    int _groupId;
    struct {
      b2Vec2 _basePos;
      b2Vec2 _leftShoulderPos;
      b2Vec2 _righShoulderPos;
      b2Vec2 _leftHipPos;
      b2Vec2 _headPos;
      b2Vec2 _neckPos;
    } _initDimensions;
  };

  // A struct used during initialization of a human to hold templates for bodies
  // and connections, not used anywhere else
  struct InitAnatomyTemplates {
    LimbBody::Config _limbTemplate;
    PolygonBody::Config _torsoTemplate;
    PolygonBody::Config _headTemplate;
    CapsuleBody::Config _neckTemplate;
    CircleBody::Config _shoulderTemplate;
    CircleBody::Config _hipTemplate;

    GirdleConnection::Config _shoulderGirdleTemplate;
    GirdleConnection::Config _hipGirdleTemplate;
  };

  InitInfo computeInitInfo(const Human::Config &config);
  InitAnatomyTemplates createAnatomyTemplates(const Human::Config &config);
  void
  createAnatomy(const Human::Config &config, const InitInfo &initInfo,
                const InitAnatomyTemplates &initAnatomyTemplates,
                const std::shared_ptr<BodyFactory> &bodyFactory,
                const std::shared_ptr<ConnectionFactory> &connectionFactory);
  void bindActions();
  void bindBehavior();

  friend class CreatureFactory;
};
