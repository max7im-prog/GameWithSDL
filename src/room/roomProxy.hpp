#pragma once
#include "entt/entity/fwd.hpp"
#include "roomIdentifiers.hpp"
#include <map>

class RoomProxy {
public:
  const RoomID &getRoomID();
  const std::string &getRoomFile();
  RoomProxy(const std::string &roomFile);
  ~RoomProxy() = default;
  void loadRoom();
  void unloadRoom();

protected:
private:
  const RoomID _id;
  const std::string _roomFile;
  bool loaded = false;
  std::map<NodeID, entt::entity> entities;

  static std::string parseRoomID(const std::string& roomFile);
};