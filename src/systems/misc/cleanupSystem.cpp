#include "cleanupSystem.hpp"
#include "eventComponents.hpp"

CleanupSystem::CleanupSystem() {}

CleanupSystem::~CleanupSystem() {}

void CleanupSystem::update(entt::registry &registry) {
  auto v = registry.view<MarkedForCleanupTag>();
  for (auto ent : v) {
    registry.destroy(ent);
  }
}
