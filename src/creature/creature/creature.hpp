#pragma once
#include "bodyPart.hpp"
#include "registryObject.hpp"
#include "world.hpp"
#include <entt/entt.hpp>

struct CreatureConfig {
  b2Vec2 position;
};

enum CreatureAbilities {
  CAN_JUMP = 1 << 0,
};

class Creature :public RegistryObject{
public:
  virtual ~Creature() = 0;

  virtual void move(b2Vec2 dir, float intensity) = 0;
  virtual void jump() {};
  virtual void attack() {};
  virtual void defend() {};
  virtual void aim([[maybe_unused]] b2Vec2 worldPoint) {};
  virtual void lookAt([[maybe_unused]] b2Vec2 worldPoint) {};

  virtual void update(float dt);

  std::uint32_t getAbilities();

protected:
  Creature(entt::registry &registry, const std::shared_ptr<World> world);

  std::uint32_t abilitiesFlags = 0;

  // Registers BodyPart in BodyParts of this Creature. Has to be called for
  // every BodyPart Creature consists of. Otherwise you are going to see issues
  // with deletiion and removal of Creatures.
  void registerBodyPart(std::shared_ptr<BodyPart> bodyPart);

  // Registers joint in joints of this Creature. Has to be called for every
  // joint Creature consists of. Otherwise you are going to see issues with
  // deletiion and removal of Creatures.
  void registerJoint(std::shared_ptr<Joint> joint);

  

private:
  const std::shared_ptr<World> world;

  std::vector<std::shared_ptr<BodyPart>> bodyParts;
  std::vector<std::shared_ptr<Joint>> joints;

  Creature() = delete;
  Creature(Creature &other) = delete;
  Creature(Creature &&other) = delete;
  Creature &operator=(Creature &other) = delete;
  Creature &operator=(Creature &&other) = delete;

  friend class CreatureFactory;
};