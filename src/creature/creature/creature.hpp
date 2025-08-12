#pragma once
#include "bodyPart.hpp"
#include "box2d/types.h"
#include "registryObject.hpp"
#include "world.hpp"
#include <entt/entt.hpp>

struct CreatureConfig {
  b2Vec2 position;
  static b2Filter defaultFilter();
};

enum CreatureAbilities {
  CAN_JUMP = 1 << 0,
};

enum CreatureState {

};

class Creature : public RegistryObject {
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

protected:
  Creature(entt::registry &registry, const std::shared_ptr<World> world);

  std::uint32_t abilitiesFlags = 0;

  const std::shared_ptr<World> world;

private:
  Creature() = delete;
  Creature(Creature &other) = delete;
  Creature(Creature &&other) = delete;
  Creature &operator=(Creature &other) = delete;
  Creature &operator=(Creature &&other) = delete;

  friend class CreatureFactory;
};