#include "creature.hpp"
#include "box2d/types.h"
#include "demoCreature.hpp"
#include "physicsUtils.hpp"
#include "registryObject.hpp"

std::uint32_t Creature::getAbilities() { return abilitiesFlags; }

Creature::Creature(entt::registry &registry, const std::shared_ptr<World> world)
    : RegistryObject(registry), world(world) {}

Creature::~Creature() {
}

void Creature::update(float dt) {
  RegistryObject::update(dt);
}

b2Filter CreatureConfig::defaultFilter(){
  b2Filter ret = b2DefaultFilter();
  ret.categoryBits = ObjectCategory::CREATURE;
  return ret;
};