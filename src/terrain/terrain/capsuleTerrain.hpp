#pragma once
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "capsuleBody.hpp"
#include "connectionFactory.hpp"
#include "terrain.hpp"

struct CapsuleTerrainConfig : public TerrainConfig {
  CapsuleBodyConfig templateBodyCfg;
  b2Vec2 point1, point2;
  float radius;
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;
};
class CapsuleTerrain : public Terrain , public Visitable<CapsuleTerrain>{
public:
  using Config = CapsuleTerrainConfig;

protected:
  CapsuleTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                 const CapsuleTerrainConfig &config,
                 const std::shared_ptr<BodyFactory> bodyFactory,
                 const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::weak_ptr<CapsuleBody> capsuleBody;

private:
  friend class TerrainFactory;
};