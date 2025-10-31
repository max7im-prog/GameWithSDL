#pragma once
#include "body.hpp"
#include "jointFactory.hpp"
#include "polygon.hpp"
#include "shapeFactory.hpp"

struct PolygonBodyConfig : public BodyConfig {
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;
  PolygonConfig shapeCfg;
};

class PolygonBody : public Body , public Visitable<PolygonBody>{
public:
  using Config = PolygonBodyConfig;
  const std::shared_ptr<Polygon> getPolygon();

protected:
  PolygonBody() = delete;
  PolygonBody(entt::registry &registry, const std::shared_ptr<World> world,
              const PolygonBodyConfig &config,
              const std::shared_ptr<ShapeFactory> shapeFactory,
              const std::shared_ptr<JointFactory> jointFactory);

  std::weak_ptr<Polygon> polygon;

  friend class BodyFactory;
};