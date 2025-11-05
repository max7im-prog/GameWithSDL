#include "roomManager.hpp"
#include "demoCreature.hpp"
#include "jsonUtils.hpp"
#include "polygonTerrain.hpp"
#include "roomIdentifiers.hpp"
#include "roomProxy.hpp"
#include <optional>
#include <string>

RoomManager::RoomManager(std::shared_ptr<World> world,
                         std::shared_ptr<CreatureFactory> creatureFactory,
                         std::shared_ptr<TerrainFactory> terrainFactory)
    : _world(world), _creatureFactory(creatureFactory),
      _terrainFactory(terrainFactory), _entities(), _rooms() {}

namespace {
EntityId createEntityId(const nlohmann::json &roomJson,
                        const nlohmann::json &entityMetadataJson) {
  EntityId ret = "";
  ret += roomJson.at("id");
  ret += "/";
  ret += entityMetadataJson.at("id");
  return ret;
}

struct EntityMetadata {
  float _x;
  float _y;
  b2Rot _rot;
  float _scaleX;
  float _scaleY;
  bool _flipX;
  bool _flipY;
};

EntityMetadata formEntityMetadata(const nlohmann::json &metadataJson,
                                  const nlohmann::json &roomJson) {
  EntityMetadata ret{};
  ret._x = roomJson.at("x").get<float>() + metadataJson.at("x").get<float>();
  ret._y = roomJson.at("y").get<float>() + metadataJson.at("y").get<float>();
  ret._rot = b2MakeRot((B2_PI * 2) * metadataJson.at("rotation").get<float>() /
                       360.0f);
  ret._scaleX = metadataJson.at("scaleX").get<float>();
  ret._scaleY = metadataJson.at("scaleY").get<float>();
  ret._flipX = metadataJson.at("flipX").get<bool>();
  ret._flipY = metadataJson.at("flipY").get<bool>();
  return ret;
}
} // namespace

const std::map<std::string,
               std::function<void(const RoomManager::EntityDispatchContext &)>>
    RoomManager::entityDispatchTable = {
        {"PolygonTerrain",
         [](const RoomManager::EntityDispatchContext &context) {
           std::string entityId =
               createEntityId(context._room->getJSON(), context._metadataJson);
           if (context._mgr._entities.contains(entityId)) {
             return;
           }
           auto metadata = formEntityMetadata(context._metadataJson,
                                              context._room->getJSON());
           PolygonTerrain::Config cfg;
           cfg.defaultConfig();
           cfg.fromJSON(context._configJson);
           cfg.position = {metadata._x, metadata._y};
           cfg.rotation = metadata._rot;

           auto factory = context._mgr._terrainFactory;
           auto ent = factory->create<PolygonTerrain>(cfg);
           context._mgr._entities.insert({entityId, ent});
         }},
        {"DemoCreature", [](const RoomManager::EntityDispatchContext &context) {
           std::string entityId =
               createEntityId(context._room->getJSON(), context._metadataJson);
           if (context._mgr._entities.contains(entityId)) {
             return;
           }
           auto metadata = formEntityMetadata(context._metadataJson,
                                              context._room->getJSON());
           DemoCreature::Config cfg;
           cfg.defaultConfig();
           cfg.fromJSON(context._configJson);
           cfg.position = {metadata._x, metadata._y};
           cfg.rotation = metadata._rot;

           auto factory = context._mgr._creatureFactory;
           auto ent = factory->create<DemoCreature>(cfg);
           context._mgr._entities.insert({entityId, ent});
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