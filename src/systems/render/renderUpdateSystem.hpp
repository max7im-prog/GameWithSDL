#pragma once

#include "entt/entity/fwd.hpp"
#include "gameSystem.hpp"
#include "renderUpdater.hpp"
class RenderUpdateSystem : public GameSystem {
public:
  RenderUpdateSystem(entt::registry &registry);
  ~RenderUpdateSystem();

  void update(entt::registry &registry, double secondsPassed);

private:
  RenderUpdater _renderUpdater;
};
