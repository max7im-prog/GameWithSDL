#pragma once
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "connectionFactory.hpp"
#include "terrain.hpp"

class SegmentTerrain : public Terrain, public VisitableImpl<SegmentTerrain> {
public:
  struct Config : public Terrain::Config {
    SegmentBody::Config templateBodyCfg;
    b2Vec2 point1, point2;
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
  SegmentTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                 const SegmentTerrain::Config &config,
                 const std::shared_ptr<BodyFactory> bodyFactory,
                 const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::weak_ptr<SegmentBody> segmentBody;

private:
  friend class TerrainFactory;
};
