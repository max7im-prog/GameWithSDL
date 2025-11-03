#include "roomManager.hpp"
#include "demoCreature.hpp"
#include "jsonUtils.hpp"
#include "polygonTerrain.hpp"
#include "roomIdentifiers.hpp"
#include "roomProxy.hpp"
#include <optional>

RoomManager::RoomManager(std::shared_ptr<World> world,
                         std::shared_ptr<CreatureFactory> creatureFactory,
                         std::shared_ptr<TerrainFactory> terrainFactory)
    : _world(world), _creatureFactory(creatureFactory),
      _terrainFactory(terrainFactory), _entities(), _rooms() {}

namespace {
EntityId createEntityId(const nlohmann::json &roomJson,
                        const nlohmann::json &entityConfigJson) {
  EntityId ret = "";
  ret += roomJson.at("id");
  ret += "/";
  ret += entityConfigJson.at("id");
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
               createEntityId(context._room->getJSON(), context._configJson);
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
         }}

        ,
        {"DemoCreature",
         [](const RoomManager::EntityDispatchContext &context) {
           std::string entityId =
               createEntityId(context._room->getJSON(), context._configJson);
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
         }}

};

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
  if (it == _rooms.end()) {
    return std::nullopt;
  }
  auto room = it->second;
  auto roomJson = room->getJSON();
  for (auto &entityMetadataJson : roomJson["entities"]) {
    auto configFile = entityMetadataJson.at("configFile").get<std::string>();
    auto temp = JsonUtils::parseJSON(configFile);
    if (!temp) {
      // TODO: log error
      continue;
    }
    auto entityConfigJson = *temp;
    auto entityType = entityConfigJson.at("type").get<std::string>();

    if (RoomManager::entityDispatchTable.contains(entityType)) {
      RoomManager::EntityDispatchContext context = {
          *this, room, entityMetadataJson, entityConfigJson};
      RoomManager::entityDispatchTable.at(std::string(entityType))(context);
    } else {
      // TODO: log error
    }
  }
  return roomId;
}