#include "renderUpdateSystem.hpp"
#include "creatureComponents.hpp"

RenderUpdateSystem::RenderUpdateSystem(entt::registry &registry)
    : GameSystem("RenderUpdateSystem"), _renderUpdater(registry) {}
RenderUpdateSystem::~RenderUpdateSystem() {}

void RenderUpdateSystem::update(entt::registry &registry,
                                double secondsPassed) {

  if (!shouldRun(secondsPassed)) {
    return;
  }

  auto v = registry.view<PhysicsCreature>();
  for (auto ent : v) {
    auto &comp = v.get<PhysicsCreature>(ent);
    auto &creature = comp.creature;
    creature->accept(_renderUpdater);
  }
}
