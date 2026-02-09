#pragma once
#include "IBehaviorModel.hpp"
#include "body.hpp"
#include "box2d/types.h"
#include "eventComponents.hpp"
#include "sceneNode.hpp"
#include "world.hpp"
#include <entt/entt.hpp>

enum class CreatureAction {
  PrimaryMobility,
  SecondaryMobility,
  PrimaryAttack,
  SecondaryAttack,
  Defensive,
  Interact,
  CreatureActionCount
};

class Creature : public SceneNode {
public:
  struct Config : SceneNode::Config {
    static b2Filter defaultFilter();
  };

  virtual ~Creature() = 0;
  virtual void move(b2Vec2 dir) = 0;
  virtual void lookAt([[maybe_unused]] b2Vec2 worldPoint) = 0;
  void perform(CreatureAction action, InputState inputState);
  virtual void update(float dt) override;

protected:
  Creature(entt::registry &registry, const std::shared_ptr<World> world);

  const std::shared_ptr<World> world;

  std::unordered_map<CreatureAction, std::function<void(InputState)>> _actions;

  void registerBehavior(std::unique_ptr<IBehaviorModel> &&behavior);
  void clearBehaviors();

private:
  std::vector<std::unique_ptr<IBehaviorModel>> _behaviors;

  Creature() = delete;
  Creature(Creature &other) = delete;
  Creature(Creature &&other) = delete;
  Creature &operator=(Creature &other) = delete;
  Creature &operator=(Creature &&other) = delete;

  friend class CreatureFactory;
};
