#include "creature.hpp"

std::uint32_t Creature::getAbilities() { return abilitiesFlags; }

Creature::Creature(entt::registry &registry, const std::shared_ptr<World> world)
    : registry(registry), world(world) {}