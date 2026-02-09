#pragma once
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "capsuleBody.hpp"
#include "connectionFactory.hpp"
#include "terrain.hpp"

class CapsuleTerrain : public Terrain, public VisitableImpl<CapsuleTerrain> {
public:
  struct Config : public Terrain::Config {
    CapsuleBody::Config templateBodyCfg;
    b2Vec2 point1, point2;
    float radius;
    void defaultConfig() override;
    void fromJSON(const nlohmann::json &json) override;
  };
  struct BodyNames {
    static constexpr std::string_view s_main = "main";

  private:
    ~BodyNames() = delete;
  };
  virtual b2Vec2 getWorldPos() override;

protected:
  CapsuleTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                 const CapsuleTerrain::Config &config,
                 const std::shared_ptr<BodyFactory> bodyFactory,
                 const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::weak_ptr<CapsuleBody> capsuleBody;

private:
  friend class TerrainFactory;
};
