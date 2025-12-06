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
      auto& renderConfig = v.get<EntityRequiresTexturingTag>(ent)._cfg;
      auto& transform = v.get<EntityRequiresTexturingTag>(ent)._sceneNodeOverallTransform;
      auto& creature = v.get<PhysicsCreature>(ent).creature;
      texturer->setupTexturing(renderConfig,transform);
      creature->accept(*texturer);
      texturer->resetTexturing();
      registry.remove<EntityRequiresTexturingTag>(ent);
    }
  }
  {
    auto v = registry.view<PhysicsTerrain, EntityRequiresTexturingTag>();
    for (auto ent : v) {
      auto& renderConfig = v.get<EntityRequiresTexturingTag>(ent)._cfg;
      auto& transform = v.get<EntityRequiresTexturingTag>(ent)._sceneNodeOverallTransform;
      auto terrain = v.get<PhysicsTerrain>(ent).terrain;
      texturer->setupTexturing(renderConfig,transform);
      terrain->accept(*texturer);
      texturer->resetTexturing();
      registry.remove<EntityRequiresTexturingTag>(ent);
    }
  }
}
