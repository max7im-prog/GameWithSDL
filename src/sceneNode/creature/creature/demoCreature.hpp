#pragma once
#include "PIDScalarController.hpp"
#include "bodyFactory.hpp"
#include "connectionFactory.hpp"
#include "creature.hpp"
#include "girdleConnection.hpp"
#include "limbBody.hpp"
#include "polygonBody.hpp"
#include <chrono>

enum Direction { LEFT, RIGHT, STANDING };

struct DemoCreatureConfig : public CreatureConfig {
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;
  float sizeXMeters = 1;
  float sizeYMeters = 1;
};

class DemoCreature : public Creature , public VisitableImpl<DemoCreature>{
public:
  using Config = DemoCreatureConfig;

  void move(b2Vec2 dir, float intensity) override;
  void aim(b2Vec2 worldPoint, bool aim) override;
  void lookAt(b2Vec2 worldPoint, bool aim) override;
  void jump() override;
  virtual void update(float dt) override;
  virtual ~DemoCreature() = default;

  b2Vec2 getWorldPos() override;

protected:
  DemoCreature(entt::registry &registry, const std::shared_ptr<World> world,
               const DemoCreatureConfig &config,
               const std::shared_ptr<BodyFactory> bodyFactory,
               const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::weak_ptr<LimbBody> _leftArm;
  std::weak_ptr<CircleBody> _leftShoulder;
  std::weak_ptr<LimbBody> _rightArm;
  std::weak_ptr<CircleBody> _rightShoulder;
  std::weak_ptr<LimbBody> _leftLeg;
  std::weak_ptr<CircleBody> _leftHip;
  std::weak_ptr<LimbBody> _rightLeg;
  std::weak_ptr<CircleBody> _rightHip;
  std::weak_ptr<PolygonBody> _torso;

  std::weak_ptr<GirdleConnection> _shoulderConnection;
  std::weak_ptr<GirdleConnection> _hipConnection;

private:
  void keepTorsoUpright(float dt);
  PIDScalarController _torsoAngleController;

  void keepTorsoAboveTheGround(float dt);
  PIDScalarController _leftLegHeightController;
  PIDScalarController _rightLegHeightController;

  void dampHorizontalMovement(float dt);
  PIDScalarController _horizontalDampingController;
  float _legHeight = 0;

  void updateJump(float dt);
  struct JumpContext {
    std::chrono::system_clock::time_point _lastJumpCall = {};
    enum JumpState { ON_GROUND, IN_AIR } _jumpState = JumpState::ON_GROUND;
    bool _jump = false;
  } _jumpContext;

  void updateMove(float dt);
  PIDScalarController _horizontalSpeedController;
  struct MoveContext {
    float _intensity;
    b2Vec2 _dir;
    bool _move;
    float _maxSpeedMultiplier = 1;
    float _defaultSpeedMpS = 10;
  } _moveContext;

  void updateFeet(float dt);
  struct FootContext {
    b2Vec2 trackingPoint = {999999, 999999};
  } _leftFootContext, _rightFootContext;
  void updateLeg(float dt, FootContext &context,
                 const std::shared_ptr<LimbBody> leg, Direction moveDir);

  struct RotationContext{
    b2Rot _rotation;

  } _rotationContext;
  friend class CreatureFactory;
};
