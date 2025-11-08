#pragma once
#include "roomManager.hpp"

#include <memory>
class RoomLoadSystem {
public:
  RoomLoadSystem() = default;
  ~RoomLoadSystem() = default;

private:
  void update(entt::registry &registry,
              std::shared_ptr<RoomManager> roomManager);
};