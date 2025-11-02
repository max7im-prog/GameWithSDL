#pragma once
#include "box2d/math_functions.h"
#include "entt/entity/fwd.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "roomIdentifiers.hpp"
#include <map>

/**
 * @brief Basically a wrapper around a nlohmann json that exposes the bounding
 * box and roomId of a room until json is actually accessed. Class loads the
 * json into memory upon access
 *
 */
class RoomProxy {
public:
  RoomProxy();
  ~RoomProxy() = default;
  const nlohmann::json &getJSON();
  const b2AABB &getAABB();
  const std::string &getRoomFile();
  const RoomId &getRoomId();
  bool isJSONLoadedIntoMemory();

  /**
   * @brief Reads the bounding box parameters and a room id from json and stores
   * it
   *
   * @param roomFile
   * @return std::optional<RoomId> Returns nullopt on failure
   */
  std::optional<RoomId> preload(std::string_view roomFile);

  /**
   * @brief removes the json from memory. Not sure if it is useful or not, but
   * still
   *
   */
  void unloadJSONFromMemory();

protected:
private:
  void loadJSONIntoMemory();
  nlohmann::json _json;
  bool _loadedIntoMemory;
  std::string _roomFile;
  b2AABB _aabb;
  RoomId _roomId;
};