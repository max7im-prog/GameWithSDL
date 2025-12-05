#include "roomProxy.hpp"
#include "jsonUtils.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include <stdexcept>
#include <string_view>

namespace {
bool canPreloadRoom(const nlohmann::json &json) {
  return json.contains("id") && json.contains("pos") &&
         json.at("pos").contains("x") && json.at("pos").contains("y") &&
         json.contains("dim") && json.at("dim").contains("h") &&
         json.at("dim").contains("w");
}
b2AABB getAABBfromJSON(const nlohmann::json &json) {
  b2AABB ret;
  float x = json.at("pos").at("x").get<float>();
  float y = json.at("pos").at("y").get<float>();
  float width = json.at("dim").at("w").get<float>();
  float height = json.at("dim").at("h").get<float>();
  ret.lowerBound = {x, y};
  ret.upperBound = {x + width, y + height};
  return ret;
}
} // namespace

RoomProxy::RoomProxy()
    : _roomFile(), _roomId(), _JSONloadedIntoMemory(false), _aabb(), _json(),
      _origin() {}

std::optional<RoomId> RoomProxy::preload(std::string_view roomFile,
                                         b2Vec2 origin, const RoomId &roomId) {
  auto temp = JsonUtils::parseJSON(std::string(roomFile));
  if (!temp) {
    return std::nullopt;
  }
  auto json = *temp;
  json["id"] = roomId;
  json["pos"] = nlohmann::basic_json<>();
  json["pos"]["x"] = origin.x;
  json["pos"]["y"] = origin.y;
  if (!canPreloadRoom(json)) {
    return std::nullopt;
  }

  _aabb = getAABBfromJSON(json);
  _origin = origin;
  _roomFile = std::string(roomFile);
  _roomId = json.at("id").get<RoomId>();
  _JSONloadedIntoMemory = false;
  _json = nlohmann::basic_json<>();

  return _roomId;
}

const nlohmann::json &RoomProxy::getJSON() const {
  if (!isJSONLoadedIntoMemory()) {
    loadJSONIntoMemory();
  }
  return _json;
}

void RoomProxy::unloadJSONFromMemory() {
  _json.clear();
  _JSONloadedIntoMemory = false;
}

void RoomProxy::loadJSONIntoMemory() const {
  auto temp = JsonUtils::parseJSON(_roomFile);
  if (!temp) {
    throw std::runtime_error("Failed to parse JSON: " + _roomFile);
  }
  _json = *temp;
  _json["pos"] = nlohmann::basic_json<>();
  _json["pos"]["x"] = _origin.x;
  _json["pos"]["y"] = _origin.y;
  _json["id"] = _roomId;
  _JSONloadedIntoMemory = true;
}

const b2AABB &RoomProxy::getAABB() const { return _aabb; }

const std::string &RoomProxy::getRoomFile() const { return _roomFile; }

const RoomId &RoomProxy::getRoomId() const { return _roomId; }

bool RoomProxy::isJSONLoadedIntoMemory() const { return _JSONloadedIntoMemory; }
