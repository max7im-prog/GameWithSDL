#include "cleanupSystem.hpp"
#include "eventComponents.hpp"
#include "gameSystem.hpp"

CleanupSystem::CleanupSystem() :GameSystem("CleanupSystem"){}

CleanupSystem::~CleanupSystem() {}

void CleanupSystem::update(entt::registry &registry, double secondsPassed) {
  if(!shouldRun(secondsPassed)){
    return;
  }
  auto v = registry.view<MarkedForCleanupTag>();
  for (auto ent : v) {
    registry.destroy(ent);
  }
}
