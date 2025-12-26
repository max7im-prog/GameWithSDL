#pragma once

#include <memory>

class Connection;

struct PhysicsConnection {
  std::shared_ptr<Connection> connection;
};
