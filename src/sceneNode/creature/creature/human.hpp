
#pragma once
#include "bodyFactory.hpp"
#include "connectionFactory.hpp"
#include "creature.hpp"
#include "humanAimModel.hpp"
#include "humanArticulationModel.hpp"
#include "humanBalanceModel.hpp"
#include "humanLocomotionModel.hpp"
#include "revoluteConnection.hpp"
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

class Human : public Creature, public VisitableImpl<Human> {
public:
  struct Config : public Creature::Config {
    void defaultConfig() override;
    void fromJSON(const nlohmann::json &json) override;
    struct {
      float _baseSizeMeters = 5.0f;

      float _shoulderGirdleWIdhtRatio = 0.3f;
      float _shoulderSizeRatio = 0.15f;

      float _hipGirdleWidthRatio = 0.3f;
      float _hipSizeRatio = 0.15f;

      float _torsoHeightRatio = 0.4;
      float _torsoWidthRatio = 0.2;

      float _headHeightRatio = 0.2f;
      float _headWidthRatio = 0.15f;

      float _limbThicknessRatio = 0.1f;
      float _neckRatio = 0.1f;
      float _leftArmRatio = 0.4f;
      float _rightArmRatio = 0.4f;
      float _leftLegRatio = 0.4f;
      float _rightLegRatio = 0.4f;
    } _proportions;

    struct {
      float _totalMassKg = 80.0f;

      float _torsoRatio = 0.3f;
      float _headRatio = 0.05f;
      float _neckRatio = 0.05f;
      float _shoulderRatio = 0.1f; // Both combined
      float _hipRatio = 0.1f;      // Both combined
      float _legRatio = 0.2f;      // Both combined
      float _armRatio = 0.2f;      // Both combined
    } _massDistribution;
  };

  struct BodyNames {
    static constexpr std::string_view s_torso = "torso";
    static constexpr std::string_view s_neck = "neck";
    static constexpr std::string_view s_head = "head";
    static constexpr std::string_view s_leftShoulder = "leftShoulder";
    static constexpr std::string_view s_rightShoulder = "rightShoulder";
    static constexpr std::string_view s_rightHip = "rightHip";
    static constexpr std::string_view s_leftHip = "leftHip";
    static constexpr std::string_view s_leftArm = "leftArm";
    static constexpr std::string_view s_rightArm = "rightArm";
    static constexpr std::string_view s_leftLeg = "leftLeg";
    static constexpr std::string_view s_rightLeg = "rightLeg";

  private:
    ~BodyNames() = delete;
  };
  struct ConnectionNames {
    static constexpr std::string_view s_shoulderGirdle = "shoulderGirdle";
    static constexpr std::string_view s_hipGirdle = "hipGirdle";
    static constexpr std::string_view s_torsoNeckConnection =
        "torsoNeckConnection";
    static constexpr std::string_view s_neckHeadConnection =
        "neckHeadConnection";

  private:
    ~ConnectionNames() = delete;
  };

  void move(b2Vec2 dir) override;
  void lookAt(b2Vec2 worldPoint) override;
  virtual void update(float dt) override;
  virtual ~Human() = default;

  b2Vec2 getWorldPos() override;

  b2Rot getRotation() const;

protected:
  Human(entt::registry &registry, const std::shared_ptr<World> world,
        const Human::Config &config,
        const std::shared_ptr<BodyFactory> bodyFactory,
        const std::shared_ptr<ConnectionFactory> connectionFactory);

  struct {
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
  } _bodies;

  struct {
    std::weak_ptr<GirdleConnection> _shoulders;
    std::weak_ptr<GirdleConnection> _hips;
    std::weak_ptr<RevoluteConnection> _headToNeck;
    std::weak_ptr<RevoluteConnection> _neckToTorso;
  } _connections;

  struct {
    HumanLocomotionModel *_locomotion;
    HumanAimModel *_aim;
    HumanArticulationModel *_articulatein;
    HumanBalanceModel *_balance;
  } _behaviors;

private:
  /**
   * @brief A struct used during initialization of a human, not used anywhere
   * else
   */
  struct InitInfo {
    int _groupId;
    struct {
      b2Vec2 _basePos = {0, 0};
      b2Vec2 _leftShoulderPos = {0, 0};
      b2Vec2 _rightShoulderPos = {0, 0};
      b2Vec2 _leftHipPos = {0, 0};
      b2Vec2 _rightHipPos = {0, 0};
      b2Vec2 _headPos = {0, 0};
      b2Vec2 _neckPos = {0, 0};
    } _initDimensions;

    struct {
      float _torso = 0.1f;
      float _shoulder = 0.1f;
      float _hip = 0.1f;
      float _head = 0.1f;
      float _neck = 0.1f;
      float _rightLeg = 0.1f;
      float _leftLeg = 0.1f;
      float _rightArm = 0.1f;
      float _leftArm = 0.1f;
    } _densities;
  };

  /**
   * @brief A struct used during initialization of a human to hold templates for
   * bodies and connections, not used anywhere else
   */
  struct InitAnatomyTemplates {
    LimbBody::Config _limbTemplate;
    PolygonBody::Config _torsoTemplate;
    PolygonBody::Config _headTemplate;
    CapsuleBody::Config _neckTemplate;
    CircleBody::Config _shoulderTemplate;
    CircleBody::Config _hipTemplate;

    GirdleConnection::Config _shoulderGirdleTemplate;
    GirdleConnection::Config _hipGirdleTemplate;
    RevoluteConnection::Config _headToNeckTemplate;
    RevoluteConnection::Config _neckToTorsoTemplate;
  };

  InitInfo computeInitInfo(const Human::Config &config);
  InitAnatomyTemplates createAnatomyTemplates(const Human::Config &config,
                                              const InitInfo &initInfo);
  void
  createAnatomy(const Human::Config &config, const InitInfo &initInfo,
                const InitAnatomyTemplates &initAnatomyTemplates,
                const std::shared_ptr<BodyFactory> &bodyFactory,
                const std::shared_ptr<ConnectionFactory> &connectionFactory);
  void bindBehavior();
  void bindActions();

  friend class CreatureFactory;
};
