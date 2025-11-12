#include "roomLoadSystem.hpp"
#include "box2d/math_functions.h"
#include "creatureComponents.hpp"
#include "roomComponents.hpp"
#include "roomIdentifiers.hpp"

RoomLoadSystem::RoomLoadSystem() : GameSystem("RoomLoadSystem") {}

void RoomLoadSystem::update(entt::registry &registry,
                            std::shared_ptr<RoomManager> roomManager,
                            double secondsPassed) {

  if (!shouldRun(secondsPassed)) {
    return;
  }

  std::vector<b2AABB> destructionBorders;

  for (auto &creatureEnt :
       registry.view<PhysicsCreature, CreatureLoadsRoomsTag>()) {
    auto loadingCreature = registry.get<PhysicsCreature>(creatureEnt).creature;
    b2Vec2 loadCenter = loadingCreature->getWorldPos();
    const auto &RoomLoadingConfig =
        registry.get<CreatureLoadsRoomsTag>(creatureEnt);

    // Compute bounding boxes of load / unload / destroy borders
    b2AABB loadAABB = {{loadCenter.x - RoomLoadingConfig.loadBorder.sizeX / 2,
                        loadCenter.y - RoomLoadingConfig.loadBorder.sizeY / 2},
                       {loadCenter.x + RoomLoadingConfig.loadBorder.sizeX / 2,
                        loadCenter.y + RoomLoadingConfig.loadBorder.sizeY / 2}};
    b2AABB unloadAABB = {
        {loadCenter.x - RoomLoadingConfig.unloadBorder.sizeX / 2,
         loadCenter.y - RoomLoadingConfig.unloadBorder.sizeY / 2},
        {loadCenter.x + RoomLoadingConfig.unloadBorder.sizeX / 2,
         loadCenter.y + RoomLoadingConfig.unloadBorder.sizeY / 2}};
    b2AABB destroyAABB = {
        {loadCenter.x - RoomLoadingConfig.destroyBorder.sizeX / 2,
         loadCenter.y - RoomLoadingConfig.destroyBorder.sizeY / 2},
        {loadCenter.x + RoomLoadingConfig.destroyBorder.sizeX / 2,
         loadCenter.y + RoomLoadingConfig.destroyBorder.sizeY / 2}};

    destructionBorders.push_back(destroyAABB);

    // Apply loading border
    {
      for (const auto &[id, room] : roomManager->getRooms()) {
        b2AABB roomAABB = room->getAABB();
        if (b2AABB_Overlaps(loadAABB, roomAABB)) {
          loadRoomIfUnloaded(id, roomManager);
        }
      }
    }

    // Apply unloading border
    {
      for (const auto &[id, room] : roomManager->getRooms()) {
        b2AABB roomAABB = room->getAABB();
        if (!b2AABB_Overlaps(unloadAABB, roomAABB)) {
          unloadRoomIfLoaded(id, roomManager);
        }
      }
    }
  }

  // Apply destruction borders globally (entity should be outside every border
  // to be destroyed)
  {
    std::vector<EntityId> entitiesToUnload;
    for (auto &[id, ent] : roomManager->getEntities()) {

      if (auto lockedEnt = ent.lock()) {

        b2Vec2 pos = lockedEnt->getWorldPos();
        auto lockedEntAABB = b2MakeAABB(&pos, 1, 0.1);
        bool insideAny = false;

        for (auto &border : destructionBorders) {
          if (b2AABB_Contains(border, lockedEntAABB)) {
            insideAny = true;
            break;
          }
        }
        if (!insideAny) {
          entitiesToUnload.push_back(id);
        }
      }
    }
    for (auto &id : entitiesToUnload) {
      roomManager->unloadEntity(id);
    }
  }
}

void RoomLoadSystem::loadRoomIfUnloaded(const RoomId &roomId,
                                        std::shared_ptr<RoomManager> mgr) {
  bool isLoaded = loadedRooms.contains(roomId) && loadedRooms[roomId];
  if (!isLoaded) {
    mgr->loadRoom(roomId);
    loadedRooms[roomId] = true;
  }
}

void RoomLoadSystem::unloadRoomIfLoaded(const RoomId &roomId,
                                        std::shared_ptr<RoomManager> mgr) {
  bool isLoaded = loadedRooms.contains(roomId) && loadedRooms[roomId];
  if (isLoaded) {
    loadedRooms[roomId] = false;
  }
}
