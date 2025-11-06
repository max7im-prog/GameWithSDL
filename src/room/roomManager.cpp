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
    // TODO: log error
    return std::nullopt;
  }
  ret += *roomId;
  ret += "/";
  auto localEntityId =
      JsonUtils::getOptional<std::string>(entityMetadataJson, "id");
  if (!localEntityId) {
    // TODO: log error
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
             // TODO: log error
             return;
           }
           if (context._mgr._entities.contains(*entityId)) {
             return;
           }

           PolygonTerrain::Config cfg;
           cfg.defaultConfig();
           cfg.fromJSON(context._configJson);
           cfg._transform = TopLevelObjectConfig::parseObjectTransform(
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
             // TODO: log error
             return;
           }
           if (context._mgr._entities.contains(*entityId)) {
             return;
           }
           CapsuleTerrain::Config cfg;
           cfg.defaultConfig();
           cfg.fromJSON(context._configJson);
           cfg._transform = TopLevelObjectConfig::parseObjectTransform(
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
             // TODO: log error
             return;
           }
           if (context._mgr._entities.contains(*entityId)) {
             return;
           }
           SegmentTerrain::Config cfg;
           cfg.defaultConfig();
           cfg.fromJSON(context._configJson);
           cfg._transform = TopLevelObjectConfig::parseObjectTransform(
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
             // TODO: log error
             return;
           }
           if (context._mgr._entities.contains(*entityId)) {
             return;
           }
           CircleTerrain::Config cfg;
           cfg.defaultConfig();
           cfg.fromJSON(context._configJson);
           cfg._transform = TopLevelObjectConfig::parseObjectTransform(
               context._metadataJson, context._room->getJSON());

           auto factory = context._mgr._terrainFactory;
           auto ent = factory->create<CircleTerrain>(cfg);
           context._mgr._entities.insert({*entityId, ent});
         }},
        {"DemoCreature", [](const RoomManager::EntityDispatchContext &context) {
           auto entityId =
               createEntityId(context._room->getJSON(), context._metadataJson);
           if (!entityId) {
             // TODO: log error
             return;
           }
           if (context._mgr._entities.contains(*entityId)) {
             return;
           }
           DemoCreature::Config cfg;
           cfg.defaultConfig();
           cfg.fromJSON(context._configJson);
           cfg._transform = TopLevelObjectConfig::parseObjectTransform(
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
RoomManager::getRooms() {
  return _rooms;
}

const std::unordered_map<EntityId, std::weak_ptr<RegistryComposite>> &
RoomManager::getEntities() {
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
  if (it == _rooms.end())
    // TODO: log room not found
    return std::nullopt;

  auto room = it->second;
  const auto &roomJson = room->getJSON();

  auto entitiesIt = roomJson.find("entities");
  if (entitiesIt == roomJson.end() || !entitiesIt->is_array()) {
    // TODO: log error: "Room missing valid 'entities' array"
    return std::nullopt;
  }

  // Load each entity through a dispatch table
  for (auto &entityMetadataJson : *entitiesIt) {
    const auto configFile =
        JsonUtils::getOptional<std::string>(entityMetadataJson, "configFile");
    if (!configFile) {
      // TODO: log error
      continue;
    }

    const auto entityConfigJson = JsonUtils::parseJSON(*configFile);
    if (!entityConfigJson) {
      // TODO: log error
      continue;
    }

    const auto entityType =
        JsonUtils::getOptional<std::string>(*entityConfigJson, "type");
    if (!entityType) {
      // TODO: log error
      continue;
    }

    if (auto dispatchIt = RoomManager::entityDispatchTable.find(*entityType);
        dispatchIt != RoomManager::entityDispatchTable.end()) {
      RoomManager::EntityDispatchContext context{
          *this, room, entityMetadataJson, *entityConfigJson};
      try {
        dispatchIt->second(context);
      } catch (const nlohmann::json::exception &e) {
        // TODO: log json error
      } catch (const std::exception &e) {
        // TODO: log factory/runtime error
      }

    } else {
      // TODO: log: unknown entity type
    }
  }

  return roomId;
}

std::vector<RoomId>
RoomManager::preloadRoomLayout(std::string_view roomLayoutFile) {
  std::vector<RoomId> ret;
  auto temp = JsonUtils::parseJSON(std::string(roomLayoutFile));
  if (!temp) {
    // TODO: log error
    return {};
  }

  auto json = *temp;
  if (!json.contains("rooms")) {
    // TODO: log error
    return {};
  }

  for (auto &room : json["rooms"]) {
    if (!room.contains("id") || !room.contains("x") || !room.contains("y") ||
        !room.contains("configFile")) {
      // TODO: log error
      continue;
    }
    b2Vec2 origin = {JsonUtils::getOrDefault<float>(room, "x", 0.0f),
                     JsonUtils::getOrDefault<float>(room, "y", 0.0f)};
    RoomId roomId = *JsonUtils::getOptional<std::string>(room, "id");
    std::string roomConfigFile =
        *JsonUtils::getOptional<std::string>(room, "configFile");

    auto id = preloadRoom(roomConfigFile, origin, roomId);
    if (id) {
      ret.push_back(*id);
    } else {
      // TODO: log error
    }
  }
  return ret;
}