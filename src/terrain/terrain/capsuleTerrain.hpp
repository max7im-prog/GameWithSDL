#pragma once
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "capsuleBody.hpp"
#include "physicsFactory.hpp"
#include "terrain.hpp"

struct CapsuleTerrainConfig : public TerrainConfig {
  CapsuleBodyConfig bodyCfg;
  b2Vec2 point1, point2;
  float radius;
  static CapsuleTerrainConfig defaultConfig();
};
class CapsuleTerrain : public Terrain {
public:
protected:
  CapsuleTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                 const CapsuleTerrainConfig &config,
                 const std::shared_ptr<PhysicsFactory> physicsFactory,
                 const std::shared_ptr<BodyFactory> bodyFactory);

  std::shared_ptr<CapsuleBody> capsuleBody;

private:
  friend class TerrainFactory;
};