#pragma once
#include "body.hpp"
#include "box2d/types.h"
#include "objectConfig.hpp"
#include "registryComposite.hpp"
#include "sceneNode.hpp"
#include "world.hpp"
#include <entt/entt.hpp>

struct CreatureConfig : TopLevelObjectConfig {
  static b2Filter defaultFilter();
};

enum CreatureAbilities {
  CAN_JUMP = 1 << 0,
};

enum CreatureState { ON_GROUND, IN_AIR, FLYING };

class Creature : public SceneNode {
public:
  virtual ~Creature() = 0;

  virtual void move(b2Vec2 dir, float intensity) = 0;
  virtual void jump() {};
  virtual void attack() {};
  virtual void defend() {};
  virtual void aim([[maybe_unused]] b2Vec2 worldPoint,
                   [[maybe_unused]] bool aim) {};
  virtual void lookAt([[maybe_unused]] b2Vec2 worldPoint,
                      [[maybe_unused]] bool look) {};

  virtual void update(float dt) override;

  std::uint32_t getAbilities();

  void registerBody(std::weak_ptr<Body> body, const std::string &name);
  void registerConnection(std::weak_ptr<Connection> connection,
                          const std::string &name);

  const std::unordered_map<std::string, std::weak_ptr<Body>> &getBodies() const;
  const std::unordered_map<std::string, std::weak_ptr<Connection>> &getConnections() const;

protected:
  Creature(entt::registry &registry, const std::shared_ptr<World> world);

  std::uint32_t creatureAbilities = 0;
  CreatureState creatureState;

  const std::shared_ptr<World> world;

  std::unordered_map<std::string, std::weak_ptr<Body>> _bodies;
  std::unordered_map<std::string, std::weak_ptr<Connection>> _connections;

private:
  Creature() = delete;
  Creature(Creature &other) = delete;
  Creature(Creature &&other) = delete;
  Creature &operator=(Creature &other) = delete;
  Creature &operator=(Creature &&other) = delete;

  friend class CreatureFactory;
};