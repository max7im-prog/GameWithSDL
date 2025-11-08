#pragma once
#include "roomIdentifiers.hpp"
#include "roomManager.hpp"

#include <memory>
class RoomLoadSystem {
public:
  RoomLoadSystem() = default;
  ~RoomLoadSystem() = default;
  void update(entt::registry &registry,
              std::shared_ptr<RoomManager> roomManager);

private:
  std::unordered_map<RoomId, bool> loadedRooms;
  void loadRoomIfUnloaded(const RoomId& roomId, std::shared_ptr<RoomManager> mgr);
  void unloadRoomIfLoaded(const RoomId& roomId, std::shared_ptr<RoomManager> mgr);
};