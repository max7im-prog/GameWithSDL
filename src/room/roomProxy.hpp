#pragma once
#include "box2d/math_functions.h"
#include "entt/entity/fwd.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "roomIdentifiers.hpp"
#include <map>

class RoomProxy {
public:
  RoomProxy() = default;
  ~RoomProxy() = default;
  const nlohmann::json &getJSON();
  const b2AABB &getAABB();
  void preload(std::string_view roomFile);
  void unloadFromMemory();
  bool isLoaded();
  bool isValidFile();
  const std::string &getRoomFile();

protected:
private:
  void loadIntoMemory();
  nlohmann::json _json;
  bool _loadedIntoMemory;
  std::string _roomFile;
  b2AABB _aabb;
  RoomId _roomId;
};