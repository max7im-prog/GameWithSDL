#pragma once
#include "bodyFactory.hpp"
#include "connectionFactory.hpp"
#include "terrain.hpp"

struct CircleTerrainConfig : public TerrainConfig {
  CircleBodyConfig templateBodyCfg;
  float radius;
  static CircleTerrainConfig defaultConfig();
};
class CircleTerrain : public Terrain , public Visitable<CircleTerrain>{
public:
  using Config = CircleTerrainConfig;

protected:
  CircleTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                const CircleTerrainConfig &config,
                const std::shared_ptr<BodyFactory> bodyFactory,
                const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::weak_ptr<CircleBody> circleBody;

private:
  friend class TerrainFactory;
};