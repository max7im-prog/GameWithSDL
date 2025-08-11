#include "creature.hpp"
#include "registryObject.hpp"

std::uint32_t Creature::getAbilities() { return abilitiesFlags; }

Creature::Creature(entt::registry &registry, const std::shared_ptr<World> world)
    : RegistryObject(registry), world(world) {}

Creature::~Creature() {
}

void Creature::update(float dt) {
  RegistryObject::update(dt);
}