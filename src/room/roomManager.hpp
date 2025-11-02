#pragma once

#include "registryComposite.hpp"
#include "roomIdentifiers.hpp"
#include "roomProxy.hpp"
#include "world.hpp"

class RoomManager {
public:
  /**
   * @brief Creates a RoomProxy for a selected roomFile. Has to be run before
   * loadRoom() for system to work
   *
   * @param roomFile
   *
   * @return A room ID of a room in a file
   */
  std::optional<RoomId> preloadRoom(std::string_view roomFile);

  /**
   * @brief Loads all the entities in a room into world except for ones that are
   * already loaded.
   *
   * @param roomId
   */
  void loadRoom(const RoomId &roomId);

  /**
   * @brief Marks a room as not loaded. Does not delete entities in a room from
   * the registry.
   *
   * @param roomId
   */
  void unloadRoom(const RoomId &roomId);
  const std::map<RoomId, std::shared_ptr<RoomProxy>> &getRooms();
  const std::map<EntityId, std::weak_ptr<RegistryComposite>> &getEntities();
  std::shared_ptr<RoomProxy> &getRoom(const RoomId &roomId);
  std::shared_ptr<RegistryComposite> getEntity(EntityId);
  void unloadEntity(const EntityId &);

protected:
private:
  std::map<RoomId, std::shared_ptr<RoomProxy>> _rooms;
  std::map<EntityId, std::weak_ptr<RegistryComposite>> _entities;
  std::shared_ptr<World> _world;
};