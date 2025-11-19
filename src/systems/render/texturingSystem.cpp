#include "texturingSystem.hpp"
#include "creatureComponents.hpp"
#include "miscComponents.hpp"
#include "terrainComponents.hpp"

TexturingSystem::TexturingSystem() : GameSystem("TexturingSystem") {}

void TexturingSystem::update(entt::registry &registry,
                             std::shared_ptr<Texturer> texturer,
                             double secondsPassed) {
  if (!shouldRun(secondsPassed)) {
    return;
  }
  {
    auto v = registry.view<EntityRequiresTexturingTag>();
    for (auto ent : v) {
      int a = 1;
    }
  }
  {
    auto v = registry.view<PhysicsCreature, EntityRequiresTexturingTag>();
    for (auto ent : v) {
      auto renderConfig = v.get<EntityRequiresTexturingTag>(ent)._cfg;
      auto creature = v.get<PhysicsCreature>(ent).creature;
      texturer->setRenderConfig(renderConfig);
      creature->accept(*texturer);
      texturer->resetRenderConfig();
      registry.remove<EntityRequiresTexturingTag>(ent);
    }
  }
  {
    auto v = registry.view<PhysicsTerrain, EntityRequiresTexturingTag>();
    for (auto ent : v) {
      auto renderConfig = v.get<EntityRequiresTexturingTag>(ent)._cfg;
      auto terrain = v.get<PhysicsTerrain>(ent).terrain;
      texturer->setRenderConfig(renderConfig);
      terrain->accept(*texturer);
      texturer->resetRenderConfig();
      registry.remove<EntityRequiresTexturingTag>(ent);
    }
  }
}