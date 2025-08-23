#pragma once
#include "bodyPart.hpp"
#include "polygon.hpp"

struct PolygonBodyConfig : public BodyConfig {
  static PolygonBodyConfig defaultConfig();
  PolygonConfig shapeCfg;
};

class PolygonBody : public Body {
public:
  const std::shared_ptr<Polygon> getPolygon();

protected:
  PolygonBody() = delete;
  PolygonBody(entt::registry &registry, const std::shared_ptr<World> world,
             const PolygonBodyConfig &config,
             const std::shared_ptr<PhysicsFactory> physicsFactory);
             
  std::shared_ptr<Polygon> polygon;

  friend class BodyFactory;
};