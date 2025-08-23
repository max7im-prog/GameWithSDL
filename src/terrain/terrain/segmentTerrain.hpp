#pragma once
#include "bodyFactory.hpp"
#include "physicsFactory.hpp"
#include "terrain.hpp"

struct SegmentTerrainConfig : public TerrainConfig {
  SegmentBodyConfig bodyCfg;
  static SegmentTerrainConfig defaultConfig();
};
class SegmentTerrain : public Terrain {
public:
protected:
  SegmentTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                 const SegmentTerrainConfig &config,
                 const std::shared_ptr<PhysicsFactory> physicsFactory,
                 const std::shared_ptr<BodyFactory> bodyFactory);

  std::shared_ptr<SegmentBody> segmentBody;

private:
  friend class TerrainFactory;
};