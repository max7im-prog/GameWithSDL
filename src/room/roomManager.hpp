#pragma once

#include "creatureFactory.hpp"
#include "registryComposite.hpp"
#include "roomIdentifiers.hpp"
#include "roomProxy.hpp"
#include "terrainFactory.hpp"
#include "world.hpp"

class RoomManager {
public:
  RoomManager(std::shared_ptr<World> world,
              std::shared_ptr<CreatureFactory> creatureFactory,
              std::shared_ptr<TerrainFactory> terrainFactory);
  /**
   * @brief Creates a RoomProxy for a selected roomFile. Has to be run
   * before loadRoom() for system to work
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
   * @param std::optional<RoomId> Returns the RoomId of a loaded room. Returns
   * nullopt on failure
   */
  std::optional<RoomId> loadRoom(const RoomId &roomId);

  const std::map<RoomId, std::shared_ptr<RoomProxy>> &getRooms();
  const std::map<EntityId, std::weak_ptr<RegistryComposite>> &getEntities();
  std::shared_ptr<RoomProxy> getRoom(const RoomId &roomId);
  std::weak_ptr<RegistryComposite> getEntity(const EntityId &entityId);
  void unloadEntity(const EntityId &entityId);

protected:
private:
  struct EntityDispatchContext {
    RoomManager &_mgr;
    std::shared_ptr<RoomProxy> _room;
    const nlohmann::json &_metadataJson;
    const nlohmann::json &_configJson;
  };

  static const std::map<std::string,
                        std::function<void(const EntityDispatchContext &)>>
      entityDispatchTable;

  std::map<RoomId, std::shared_ptr<RoomProxy>> _rooms;
  std::map<EntityId, std::weak_ptr<RegistryComposite>> _entities;
  const std::shared_ptr<World> _world;
  const std::shared_ptr<CreatureFactory> _creatureFactory;
  const std::shared_ptr<TerrainFactory> _terrainFactory;
};