#include "roomManager.hpp"
#include "roomProxy.hpp"
#include <optional>

std::optional<RoomId> RoomManager::preloadRoom(std::string_view roomFile) {
  std::shared_ptr<RoomProxy> room = std::make_shared<RoomProxy>();
  auto roomId = room->preload(roomFile);
  if (!roomId) {
    return std::nullopt;
  }
  _rooms[*roomId] = room;
  return roomId;
}

const std::map<RoomId, std::shared_ptr<RoomProxy>> &RoomManager::getRooms() {
  return _rooms;
}

const std::map<EntityId, std::weak_ptr<RegistryComposite>> &
RoomManager::getEntities() {
  return _entities;
}

std::shared_ptr<RoomProxy> RoomManager::getRoom(const RoomId &roomId) {
  return _rooms[roomId];
}

std::weak_ptr<RegistryComposite>
RoomManager::getEntity(const EntityId &entityId) {
  return _entities[entityId];
}

void RoomManager::unloadEntity(const EntityId &entityId) {
  auto it = _entities.find(entityId);
  if (it == _entities.end()) {
    return;
  }
  auto entityPtr = it->second;
  _world->removeObject(entityPtr);
  _entities.erase(it);
}