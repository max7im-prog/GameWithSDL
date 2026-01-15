#pragma once
#include "world.hpp"

struct BasicWorldConfig : public WorldConfig {
  static BasicWorldConfig defaultConfig();
};

class BasicWorld : public World {
public:
  using Config = BasicWorldConfig;
  BasicWorld(entt::registry &registry, const BasicWorldConfig &config);
  ~BasicWorld() = default;

private:
  friend class WorldFactory;
};