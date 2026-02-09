#pragma once
#include "body.hpp"
#include "jointFactory.hpp"
#include "polygon.hpp"
#include "shapeFactory.hpp"

class PolygonBody : public Body, public VisitableImpl<PolygonBody> {
public:
  struct Config : public Body::Config {
    void defaultConfig() override;
    Polygon::Config shapeCfg;
  };
  struct ShapeNames {
    static constexpr std::string_view s_main = "main";

  private:
    ~ShapeNames() = delete;
  };
  const std::shared_ptr<Polygon> getPolygon();
  virtual b2Vec2 getWorldPos() override;

protected:
  PolygonBody() = delete;
  PolygonBody(entt::registry &registry, const std::shared_ptr<World> world,
              const PolygonBody::Config &config,
              const std::shared_ptr<ShapeFactory> shapeFactory,
              const std::shared_ptr<JointFactory> jointFactory);

  std::weak_ptr<Polygon> polygon;

  friend class BodyFactory;
};
