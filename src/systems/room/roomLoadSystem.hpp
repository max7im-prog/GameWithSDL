#pragma once
#include "gameSystem.hpp"
#include "roomIdentifiers.hpp"
#include "roomManager.hpp"

#include <memory>
class RoomLoadSystem :public GameSystem{
public:
  RoomLoadSystem() ;
  ~RoomLoadSystem() = default;

  // FIXME: implement proper loading for multiple creatures
  void update(entt::registry &registry,
              std::shared_ptr<RoomManager> roomManager);

private:
  std::unordered_map<RoomId, bool> loadedRooms;
  void loadRoomIfUnloaded(const RoomId& roomId, std::shared_ptr<RoomManager> mgr);
  void unloadRoomIfLoaded(const RoomId& roomId, std::shared_ptr<RoomManager> mgr);
};