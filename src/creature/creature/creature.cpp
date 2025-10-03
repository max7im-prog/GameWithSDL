#include "creature.hpp"
#include "box2d/types.h"
#include "demoCreature.hpp"
#include "physicsUtils.hpp"
#include "registryComposite.hpp"

std::uint32_t Creature::getAbilities() { return creatureAbilities; }

Creature::Creature(entt::registry &registry, const std::shared_ptr<World> world)
    : RegistryComposite(registry), world(world) {}

Creature::~Creature() {}

void Creature::update(float dt) { RegistryComposite::update(dt); }

b2Filter CreatureConfig::defaultFilter() {
  b2Filter ret = b2DefaultFilter();
  ret.categoryBits = ObjectCategory::CREATURE;
  return ret;
};
