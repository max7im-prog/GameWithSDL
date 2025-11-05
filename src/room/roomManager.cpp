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

TopLevelObjectConfig::Transform
formEntityTransform(const nlohmann::json &metadataJson,
                    const nlohmann::json &roomJson) {
  TopLevelObjectConfig::Transform ret{};

  // Position
  float roomX = JsonUtils::getOrDefault<float>(roomJson, "x", 0.0f);
  float roomY = JsonUtils::getOrDefault<float>(roomJson, "y", 0.0f);
  float offsetX = JsonUtils::getOrDefault<float>(metadataJson, "x", 0.0f);
  float offsetY = JsonUtils::getOrDefault<float>(metadataJson, "y", 0.0f);
  ret._originPos = {roomX, roomY};
  ret._relativePos = {offsetX, offsetY};

  // Rotation
  float rotationAngle =
      JsonUtils::getOrDefault<float>(metadataJson, "rotation", 0);
  float rootAngle = 0;
  ret._relativeRot = b2MakeRot((B2_PI * 2) * rotationAngle / 360.0f);
  ret._rootRot = b2MakeRot((B2_PI * 2) * rootAngle / 360.0f);

  // Scale
  ret._scaleX = JsonUtils::getOrDefault<float>(metadataJson, "scaleX", 1.0f);
  ret._scaleY = JsonUtils::getOrDefault<float>(metadataJson, "scaleY", 1.0f);

  // Flip
  ret._flipX = JsonUtils::getOrDefault<bool>(metadataJson, "flipX", false);
  ret._flipY = JsonUtils::getOrDefault<bool>(metadataJson, "flipY", false);

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
           cfg._transform = formEntityTransform(context._metadataJson,
                                                context._room->getJSON());

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
           cfg._transform = formEntityTransform(context._metadataJson,
                                                context._room->getJSON());

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
           cfg._transform = formEntityTransform(context._metadataJson,
                                                context._room->getJSON());

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
           cfg._transform = formEntityTransform(context._metadataJson,
                                                context._room->getJSON());

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
           cfg._transform = formEntityTransform(context._metadataJson,
                                                context._room->getJSON());

           auto factory = context._mgr._creatureFactory;
           auto ent = factory->create<DemoCreature>(cfg);
           context._mgr._entities.insert({*entityId, ent});
         }}};

std::optional<RoomId> RoomManager::preloadRoom(std::string_view roomFile) {
  std::shared_ptr<RoomProxy> room = std::make_shared<RoomProxy>();
  auto roomId = room->preload(roomFile);
  if (!roomId) {
    return std::nullopt;
  }
  _rooms[*roomId] = room;
  return roomId;
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