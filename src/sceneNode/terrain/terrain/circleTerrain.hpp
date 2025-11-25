#pragma once
#include "bodyFactory.hpp"
#include "connectionFactory.hpp"
#include "terrain.hpp"

struct CircleTerrainConfig : public TerrainConfig {
  CircleBodyConfig templateBodyCfg;
  float radius;
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;
};
class CircleTerrain : public Terrain , public VisitableImpl<CircleTerrain>{
public:
  using Config = CircleTerrainConfig;
  virtual b2Vec2 getWorldPos() override;

protected:
  CircleTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                const CircleTerrainConfig &config,
                const std::shared_ptr<BodyFactory> bodyFactory,
                const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::weak_ptr<CircleBody> circleBody;

private:
  friend class TerrainFactory;
};