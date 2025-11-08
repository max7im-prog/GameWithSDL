#include "roomLoadSystem.hpp"
#include "creatureComponents.hpp"
#include "roomComponents.hpp"
#include <exception>
void RoomLoadSystem::update(entt::registry &registry,
                            std::shared_ptr<RoomManager> roomManager) {
  for (auto &creatureEnt : registry.view<CreatureLoadsRoomsTag>()) {
    b2Vec2 creaturePos{0, 0};
    try {
      auto &creature = registry.get<PhysicsCreature>(creatureEnt).creature;
      creaturePos = creature->getWorldPos();
    } catch (std::exception &e) {
      // TODO: log error
      continue;
    }
    for (auto &terrainEnt : registry.view<PhysicsTerrain>()) {
      try {
        auto &terrain = registry.get<PhysicsTerrain>(terrainEnt).terrain;
        // b2Vec2 terrainPos = terrain->getWorldPos();
      } catch (...) {
        // TODO: log error
      }
    }
  }
}