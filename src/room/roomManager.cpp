#include "roomManager.hpp"
#include "demoCreature.hpp"
#include "jsonUtils.hpp"
#include "objectConfig.hpp"
#include "polygonTerrain.hpp"
#include "roomIdentifiers.hpp"
#include "roomProxy.hpp"
#include "segmentTerrain.hpp"
#include <optional>
#include <string>

RoomManager::RoomManager(std::shared_ptr<World> world,
                         std::shared_ptr<CreatureFactory> creatureFactory,
                         std::shared_ptr<TerrainFactory> terrainFactory)
    : _world(world), _creatureFactory(creatureFactory),
      _terrainFactory(terrainFactory), _entities(), _rooms() {}

namespace {
std::optional<EntityId>
createEntityId(const nlohmann::json &roomJson,
               const nlohmann::json &entityMetadataJson) {
  EntityId ret = "";
  auto roomId = JsonUtils::getOptional<std::string>(roomJson, "id");
  if (!roomId) {
    spdlog::error("Room JSON missing required field 'id'");
    return std::nullopt;
  }
  ret += *roomId;
  ret += "/";
  auto localEntityId =
      JsonUtils::getOptional<std::string>(entityMetadataJson, "id");
  if (!localEntityId) {
    spdlog::error("Entity metadata in room '{}' missing required field 'id'",
                  *roomId);
    return std::nullopt;
  }
  ret += *localEntityId;
  return ret;
}

} // namespace

const std::map<std::string,
               std::function<void(const RoomManager::EntityDispatchContext &)>>
    RoomManager::entityDispatchTable = {
        {"PolygonTerrain",
         [](const RoomManager::EntityDispatchContext &context) {
           auto entityId =
               createEntityId(context._room->getJSON(), context._metadataJson);
           if (!entityId) {

             spdlog::error("RoomManager: failed to parse entity id for "
                           "PolygonTerrain in a room "
                           "with ID = '{}'",
                           context._room->getRoomId());
             return;
           }
           if (context._mgr._entities.contains(*entityId)) {
             spdlog::debug("Duplicate entity id '{}' in room '{}', skipping",
                           *entityId, context._room->getRoomId());
             return;
           }

           PolygonTerrain::Config cfg;
           cfg.defaultConfig();
           cfg.fromJSON(context._configJson);
           cfg._transform = SceneNodeConfig::parseObjectTransform(
               context._metadataJson, context._room->getJSON());

           auto factory = context._mgr._terrainFactory;
           auto ent = factory->create<PolygonTerrain>(cfg);
           context._mgr._entities.insert({*entityId, ent});
         }},
        {"CapsuleTerrain",
         [](const RoomManager::EntityDispatchContext &context) {
           auto entityId =
               createEntityId(context._room->getJSON(), context._metadataJson);
           if (!entityId) {
             spdlog::error("RoomManager: failed to parse entity id for "
                           "CapsuleTerrain in a room "
                           "with ID = '{}'",
                           context._room->getRoomId());
             return;
           }
           if (context._mgr._entities.contains(*entityId)) {
             spdlog::debug("Duplicate entity id '{}' in room '{}', skipping",
                           *entityId, context._room->getRoomId());
             return;
           }
           CapsuleTerrain::Config cfg;
           cfg.defaultConfig();
           cfg.fromJSON(context._configJson);
           cfg._transform = SceneNodeConfig::parseObjectTransform(
               context._metadataJson, context._room->getJSON());

           auto factory = context._mgr._terrainFactory;
           auto ent = factory->create<CapsuleTerrain>(cfg);
           context._mgr._entities.insert({*entityId, ent});
         }},
        {"SegmentTerrain",
         [](const RoomManager::EntityDispatchContext &context) {
           auto entityId =
               createEntityId(context._room->getJSON(), context._metadataJson);
           if (!entityId) {
             spdlog::error("RoomManager: failed to parse entity id for "
                           "SegmantTerrain in a room "
                           "with ID = '{}'",
                           context._room->getRoomId());
             return;
           }
           if (context._mgr._entities.contains(*entityId)) {
             spdlog::debug("Duplicate entity id '{}' in room '{}', skipping",
                           *entityId, context._room->getRoomId());
             return;
           }
           SegmentTerrain::Config cfg;
           cfg.defaultConfig();
           cfg.fromJSON(context._configJson);
           cfg._transform = SceneNodeConfig::parseObjectTransform(
               context._metadataJson, context._room->getJSON());

           auto factory = context._mgr._terrainFactory;
           auto ent = factory->create<SegmentTerrain>(cfg);
           context._mgr._entities.insert({*entityId, ent});
         }},
        {"CircleTerrain",
         [](const RoomManager::EntityDispatchContext &context) {
           auto entityId =
               createEntityId(context._room->getJSON(), context._metadataJson);
           if (!entityId) {
             spdlog::error("RoomManager: failed to parse entity id for "
                           "CircleTerrain in a room "
                           "with ID = '{}'",
                           context._room->getRoomId());
             return;
           }
           if (context._mgr._entities.contains(*entityId)) {
             spdlog::debug("Duplicate entity id '{}' in room '{}', skipping",
                           *entityId, context._room->getRoomId());
             return;
           }
           CircleTerrain::Config cfg;
           cfg.defaultConfig();
           cfg.fromJSON(context._configJson);
           cfg._transform = SceneNodeConfig::parseObjectTransform(
               context._metadataJson, context._room->getJSON());

           auto factory = context._mgr._terrainFactory;
           auto ent = factory->create<CircleTerrain>(cfg);
           context._mgr._entities.insert({*entityId, ent});
         }},
        {"DemoCreature", [](const RoomManager::EntityDispatchContext &context) {
           auto entityId =
               createEntityId(context._room->getJSON(), context._metadataJson);
           if (!entityId) {
             spdlog::error("RoomManager: failed to parse entity id for "
                           "DemoCreature in a room "
                           "with ID = '{}'",
                           context._room->getRoomId());
             return;
           }
           if (context._mgr._entities.contains(*entityId)) {
             spdlog::debug("Duplicate entity id '{}' in room '{}', skipping",
                           *entityId, context._room->getRoomId());
             return;
           }
           DemoCreature::Config cfg;
           cfg.defaultConfig();
           cfg.fromJSON(context._configJson);
           cfg._transform = SceneNodeConfig::parseObjectTransform(
               context._metadataJson, context._room->getJSON());

           auto factory = context._mgr._creatureFactory;
           auto ent = factory->create<DemoCreature>(cfg);
           context._mgr._entities.insert({*entityId, ent});
         }}};

std::optional<RoomId> RoomManager::preloadRoom(std::string_view roomFile,
                                               b2Vec2 origin,
                                               const RoomId &roomId) {
  std::shared_ptr<RoomProxy> room = std::make_shared<RoomProxy>();
  auto id = room->preload(roomFile, origin, roomId);
  if (!id) {
    return std::nullopt;
  }
  _rooms[*id] = room;
  return id;
}

const std::unordered_map<RoomId, std::shared_ptr<RoomProxy>> &
RoomManager::getRooms() const {
  return _rooms;
}

const std::unordered_map<EntityId, std::weak_ptr<SceneNode>> &
RoomManager::getEntities() const {
  return _entities;
}

std::shared_ptr<RoomProxy> RoomManager::getRoom(const RoomId &roomId) {
  return _rooms[roomId];
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

std::optional<RoomId> RoomManager::loadRoom(const RoomId &roomId) {
  auto it = _rooms.find(roomId);
  if (it == _rooms.end()) {
    spdlog::warn(
        "RoomManager: room with id '{}' is not preloaded into memory, skipping",
        roomId);
    return std::nullopt;
  }

  auto room = it->second;
  const auto &roomJson = room->getJSON();

  auto entitiesIt = roomJson.find("entities");
  if (entitiesIt == roomJson.end() || !entitiesIt->is_array()) {
    spdlog::error("RoomManager: room with id '{}' is missing a valid "
                  "'entities' array, skipping",
                  roomId);
    return std::nullopt;
  }

  // Load each entity through a dispatch table
  for (auto &entityMetadataJson : *entitiesIt) {
    const auto configFile =
        JsonUtils::getOptional<std::string>(entityMetadataJson, "configFile");
    if (!configFile) {
      spdlog::error(
          "RoomManager: Entity with missing config file in room with id = '{}'"
          ", skipping",
          roomId);
      continue;
    }

    const auto entityConfigJson = JsonUtils::parseJSON(*configFile);
    if (!entityConfigJson) {
      spdlog::error("RoomManager: failed to parse config file: '{}' for an "
                    "entity in room with id = '{}'",
                    *configFile, roomId);
      continue;
    }

    const auto entityType =
        JsonUtils::getOptional<std::string>(*entityConfigJson, "type");
    if (!entityType) {
      spdlog::error(
          "RoomManager: missing entity type in config file: '{}' for an "
          "entity in room with id = '{}'",
          *configFile, roomId);
      continue;
    }

    if (auto dispatchIt = RoomManager::entityDispatchTable.find(*entityType);
        dispatchIt != RoomManager::entityDispatchTable.end()) {
      RoomManager::EntityDispatchContext context{
          *this, room, entityMetadataJson, *entityConfigJson};
      try {
        dispatchIt->second(context);
      } catch (const nlohmann::json::exception &e) {
        spdlog::error(
            "JSON error while loading entity from '{}' in room '{}': {}",
            *configFile, roomId, e.what());
      } catch (const std::exception &e) {
        spdlog::error(
            "Failed to create entity of type '{}' from '{}' in room '{}': {}",
            *entityType, *configFile, roomId, e.what());
      }

    } else {
      spdlog::error("Unknown entity type '{}' in config '{}' (room '{}')",
                    *entityType, *configFile, roomId);
    }
  }

  return roomId;
}

std::vector<RoomId>
RoomManager::preloadRoomLayout(std::string_view roomLayoutFile) {
  std::vector<RoomId> ret;
  auto temp = JsonUtils::parseJSON(std::string(roomLayoutFile));
  if (!temp) {
    spdlog::error("RoomManager: Failed to parse room layout file '{}'",
                  roomLayoutFile);
    return {};
  }

  auto json = *temp;
  if (!json.contains("rooms")) {
    spdlog::error(
        "RoomManager: Room layout '{}' missing required 'rooms' array",
        roomLayoutFile);
    return {};
  }

  for (auto &room : json["rooms"]) {
    if (!room.contains("id") || !room.contains("pos") ||
        !room.contains("configFile")) {
      spdlog::error(
          "RoomManager: Room entry in layout '{}' missing required fields (id, "
          "pos, configFile)",
          roomLayoutFile);
      continue;
    }
    auto pos = room["pos"];

    b2Vec2 origin = {JsonUtils::getOrDefault<float>(pos, "x", 0.0f),
                     JsonUtils::getOrDefault<float>(pos, "y", 0.0f)};
    RoomId roomId = *JsonUtils::getOptional<std::string>(room, "id");

    auto roomConfigFile =
        JsonUtils::getOptional<std::string>(room, "configFile");
    if (!roomConfigFile) {
      spdlog::error("RoomManager: Room entry in world layout '{}' missing "
                    "required field 'configFile'",
                    roomLayoutFile);
      continue;
    }

    auto id = preloadRoom(*roomConfigFile, origin, roomId);
    if (id) {
      ret.push_back(*id);
    } else {
      spdlog::error("Failed to preload room '{}' from config '{}'", roomId,
                    *roomConfigFile);
    }
  }
  return ret;
}
