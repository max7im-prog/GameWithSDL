#pragma once
#include "IBehaviorModel.hpp"
#include "body.hpp"
#include "box2d/types.h"
#include "eventComponents.hpp"
#include "sceneNode.hpp"
#include "world.hpp"
#include <cstddef>
#include <entt/entt.hpp>

struct CreatureConfig : SceneNodeConfig {
  static b2Filter defaultFilter();
};

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
  virtual ~Creature() = 0;
  virtual void move(b2Vec2 dir) = 0;
  virtual void lookAt([[maybe_unused]] b2Vec2 worldPoint) = 0;
  void perform(CreatureAction action, InputState inputState);
  virtual void update(float dt) override;

protected:
  Creature(entt::registry &registry, const std::shared_ptr<World> world);

  void registerBody(std::weak_ptr<Body> body, const std::string &name);
  void registerConnection(std::weak_ptr<Connection> connection,
                          const std::string &name);

  const std::shared_ptr<World> world;

  std::unordered_map<CreatureAction, std::function<void(InputState)>> _actions;

  void registerBehavior(std::shared_ptr<IBehaviorModel> behavior);
  void clearBehaviors();

private:
  std::vector<std::shared_ptr<IBehaviorModel>> _behaviors;

  Creature() = delete;
  Creature(Creature &other) = delete;
  Creature(Creature &&other) = delete;
  Creature &operator=(Creature &other) = delete;
  Creature &operator=(Creature &&other) = delete;

  friend class CreatureFactory;
};
