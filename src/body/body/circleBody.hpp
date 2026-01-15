#pragma once
#include "body.hpp"
#include "circle.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"

class CircleBody : public Body, public VisitableImpl<CircleBody> {
public:
  struct Config : public Body::Config {
    void defaultConfig() override;
    CircleConfig shapeCfg;
  };
  b2Vec2 getCenter();
  float getRadius();
  const std::shared_ptr<Circle> getCircle();
  virtual b2Vec2 getWorldPos() override;

protected:
  CircleBody() = delete;
  CircleBody(entt::registry &registry, const std::shared_ptr<World> world,
             const CircleBody::Config &config,
             const std::shared_ptr<ShapeFactory> shapeFactory,
             const std::shared_ptr<JointFactory> jointFactory);
  std::weak_ptr<Circle> circle;

  friend class BodyFactory;
};
