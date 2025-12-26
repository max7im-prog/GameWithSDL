#pragma once

#include <memory>
class Body;

struct PhysicsBody {
  std::shared_ptr<Body> body;
};
