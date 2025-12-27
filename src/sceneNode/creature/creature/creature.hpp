#pragma once
#include "body.hpp"
#include "box2d/types.h"
#include "sceneNode.hpp"
#include "world.hpp"
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
  virtual void move(const b2Vec2 &dir) = 0;
  virtual void lookAt([[maybe_unused]] const b2Vec2 &worldPoint,
                      [[maybe_unused]] bool look) {};
  virtual void perform(CreatureAction action, bool pressed);
  virtual void update(float dt) override;

protected:
  Creature(entt::registry &registry, const std::shared_ptr<World> world);

  void registerBody(std::weak_ptr<Body> body, const std::string &name);
  void registerConnection(std::weak_ptr<Connection> connection,
                          const std::string &name);

  const std::shared_ptr<World> world;

private:
  Creature() = delete;
  Creature(Creature &other) = delete;
  Creature(Creature &&other) = delete;
  Creature &operator=(Creature &other) = delete;
  Creature &operator=(Creature &&other) = delete;

  friend class CreatureFactory;
};
