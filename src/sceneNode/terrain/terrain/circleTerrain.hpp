#pragma once
#include "bodyFactory.hpp"
#include "connectionFactory.hpp"
#include "terrain.hpp"

class CircleTerrain : public Terrain, public VisitableImpl<CircleTerrain> {
public:
  struct Config : public Terrain::Config {
    CircleBody::Config templateBodyCfg;
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
  CircleTerrain(entt::registry &registry, const std::shared_ptr<World> world,
                const CircleTerrain::Config &config,
                const std::shared_ptr<BodyFactory> bodyFactory,
                const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::weak_ptr<CircleBody> circleBody;

private:
  friend class TerrainFactory;
};
