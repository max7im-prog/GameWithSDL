#pragma once
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "physicsFactory.hpp"
#include "terrain.hpp"

struct SegmentTerrainConfig : public TerrainConfig {
  SegmentBodyConfig templateBodyCfg;
  b2Vec2 point1, point2;
  static SegmentTerrainConfig defaultConfig();
};
class SegmentTerrain : public Terrain {
public:
  using Config = SegmentTerrainConfig;

protected:
  SegmentTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                 const SegmentTerrainConfig &config,
                 const std::shared_ptr<PhysicsFactory> physicsFactory,
                 const std::shared_ptr<BodyFactory> bodyFactory);

  std::shared_ptr<SegmentBody> segmentBody;

private:
  friend class TerrainFactory;
};