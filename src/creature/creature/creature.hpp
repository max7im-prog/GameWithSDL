#pragma once
#include "bodyPart.hpp"
#include "world.hpp"
#include <entt/entt.hpp>

struct creatureConfig {};

enum CreatureAbilities {
  CAN_JUMP = 1 << 0,
};

class Creature {
public:
  virtual ~Creature();

  virtual void move(b2Vec2 dir, float intensity) = 0;
  virtual void jump() {};
  virtual void attack() {};
  virtual void defend() {};
  virtual void aim([[maybe_unused]] b2Vec2 worldPoint) {};
  virtual void lookAt([[maybe_unused]] b2Vec2 worldPoint) {};

  virtual void update() = 0;

  std::uint32_t getAbilities();

protected:
  Creature(entt::registry &registry, const std::shared_ptr<World> world);

  std::uint32_t abilitiesFlags = 0;

  
  void registerBodyPart(std::shared_ptr<BodyPart> bodyPart);
  void registerJoint(std::shared_ptr<Joint> joint);

private:
  entt::registry &registry;
  const std::shared_ptr<World> world;

  std::set<std::shared_ptr<BodyPart>> bodyParts;
  std::set<std::shared_ptr<Joint>> joints;

  Creature() = delete;
  Creature(Creature &other) = delete;
  Creature(Creature &&other) = delete;
  Creature &operator=(Creature &other) = delete;
  Creature &operator=(Creature &&other) = delete;
};