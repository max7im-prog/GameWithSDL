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
  static DemoCreatureConfig defaultConfig();
  float sizeXMeters = 1;
  float sizeYMeters = 1;
};

class DemoCreature : public Creature {
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

  std::weak_ptr<LimbBody> leftArm;
  std::weak_ptr<CircleBody> leftShoulder;
  std::weak_ptr<LimbBody> rightArm;
  std::weak_ptr<CircleBody> rightShoulder;
  std::weak_ptr<LimbBody> leftLeg;
  std::weak_ptr<CircleBody> leftHip;
  std::weak_ptr<LimbBody> rightLeg;
  std::weak_ptr<CircleBody> rightHip;
  std::weak_ptr<PolygonBody> torso;

  std::weak_ptr<GirdleConnection> shoulderConnection;
  std::weak_ptr<GirdleConnection> hipConnection;

private:
  void keepTorsoUpright(float dt);
  PIDScalarController torsoAngleController;

  void keepTorsoAboveTheGround(float dt);
  PIDScalarController leftLegHeightController;
  PIDScalarController rightLegHeightController;

  void dampHorizontalMovement(float dt);
  PIDScalarController horizontalDampingController;
  float legHeight = 0;

  void updateJump(float dt);
  struct JumpContext {
    std::chrono::system_clock::time_point lastJumpCall = {};
    enum JumpState { ON_GROUND, IN_AIR } jumpState = JumpState::ON_GROUND;
    bool jump = false;
  } jumpContext;

  void updateMove(float dt);
  PIDScalarController horizontalSpeedController;
  struct MoveContext {
    float intensity;
    b2Vec2 dir;
    bool move;
    float maxSpeedMultiplier = 1;
    float defaultSpeedMpS = 10;
  } moveContext;

  void updateFeet(float dt);
  struct FootContext {
    b2Vec2 trackingPoint = {999999, 999999};
  } leftFootContext, rightFootContext;
  void updateLeg(float dt, FootContext &context,
                 const std::shared_ptr<LimbBody> leg, Direction moveDir);

  friend class CreatureFactory;
};