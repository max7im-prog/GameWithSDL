#pragma once
#include "PIDScalarController.hpp"
#include "bodyFactory.hpp"
#include "creature.hpp"
#include "limbBody.hpp"
#include "physicsFactory.hpp"
#include "polygonBody.hpp"
#include "revoluteJoint.hpp"
#include <chrono>

struct DemoCreatureConfig : public CreatureConfig {
  static DemoCreatureConfig defaultConfig();
  float sizeXMeters = 1;
  float sizeYMeters = 1;
};

class DemoCreature : public Creature {
public:
  void move(b2Vec2 dir, float intensity) override;
  void aim(b2Vec2 worldPoint, bool aim) override;
  void jump() override;
  virtual void update(float dt) override;
  virtual ~DemoCreature() = default;

protected:
  DemoCreature(entt::registry &registry, const std::shared_ptr<World> world,
               const DemoCreatureConfig &config,
               const std::shared_ptr<PhysicsFactory> physicsFactory,
               const std::shared_ptr<BodyFactory> bodyFactory);

  std::shared_ptr<LimbBody> leftArm = nullptr;
  std::shared_ptr<LimbBody> rightArm = nullptr;
  std::shared_ptr<LimbBody> leftLeg = nullptr;
  std::shared_ptr<LimbBody> rightLeg = nullptr;
  std::shared_ptr<PolygonBody> torso = nullptr;

  std::shared_ptr<RevoluteJoint> leftShoulderJoint = nullptr;
  std::shared_ptr<RevoluteJoint> rightShoulderJoint = nullptr;
  std::shared_ptr<RevoluteJoint> leftHipJoint = nullptr;
  std::shared_ptr<RevoluteJoint> rightHipJoint = nullptr;

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
  struct MoveContext{
    float intensity;
    b2Vec2 dir;
    bool move;
    float maxSpeedMultiplier = 1;
    float defalutSpeedMpS = 10;
  }moveContext;

  friend class CreatureFactory;
};