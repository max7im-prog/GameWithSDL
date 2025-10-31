#pragma once
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "connectionFactory.hpp"
#include "terrain.hpp"

struct SegmentTerrainConfig : public TerrainConfig {
  SegmentBodyConfig templateBodyCfg;
  b2Vec2 point1, point2;
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;
};
class SegmentTerrain : public Terrain, public Visitable<SegmentTerrain> {
public:
  using Config = SegmentTerrainConfig;

protected:
  SegmentTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                 const SegmentTerrainConfig &config,
                 const std::shared_ptr<BodyFactory> bodyFactory,
                 const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::weak_ptr<SegmentBody> segmentBody;

private:
  friend class TerrainFactory;
};