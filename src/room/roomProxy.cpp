#include "roomProxy.hpp"
#include "jsonUtils.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include <stdexcept>
#include <string_view>

namespace {
bool canPreload(const nlohmann::json &json) {
  return json.contains("id") && json.contains("x") && json.contains("y") &&
         json.contains("height") && json.contains("width");
}
b2AABB getAABBfromJSON(const nlohmann::json &json) {
  b2AABB ret;
  float x = json.at("x").get<float>();
  float y = json.at("y").get<float>();
  float width = json.at("width").get<float>();
  float height = json.at("height").get<float>();
  ret.lowerBound = {x, y};
  ret.upperBound = {x + width, y + height};
  return ret;
}
} // namespace

RoomProxy::RoomProxy()
    : _roomFile(), _roomId(), _loadedIntoMemory(false), _aabb(), _json() {}

std::optional<RoomId> RoomProxy::preload(std::string_view roomFile) {
  auto temp = JsonUtils::parseJSON(std::string(roomFile));
  if (!temp) {
    return std::nullopt;
  }
  auto json = *temp;
  if (!canPreload(json)) {
    return std::nullopt;
  }

  _aabb = getAABBfromJSON(json);
  _roomFile = std::string(roomFile);
  _roomId = json.at("id").get<RoomId>();
  _loadedIntoMemory = false;
  _json = nlohmann::basic_json<>();

  return _roomId;
}

const nlohmann::json &RoomProxy::getJSON() {
  if (!isJSONLoadedIntoMemory()) {
    loadJSONIntoMemory();
  }
  return _json;
}

void RoomProxy::unloadJSONFromMemory() {
  _json.clear();
  _loadedIntoMemory = false;
}

void RoomProxy::loadJSONIntoMemory() {
  auto temp = JsonUtils::parseJSON(_roomFile);
  if (!temp) {
    throw std::runtime_error("Failed to parse JSON: " + _roomFile);
  }
  _json = *temp;
  _loadedIntoMemory = true;
}

const b2AABB &RoomProxy::getAABB() { return _aabb; }

const std::string &RoomProxy::getRoomFile() { return _roomFile; }

const RoomId &RoomProxy::getRoomId() { return _roomId; }