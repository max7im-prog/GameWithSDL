#pragma once
#include "body.hpp"
#include "circle.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"

struct CircleBodyConfig : public BodyConfig {
  static CircleBodyConfig defaultConfig();
  CircleConfig shapeCfg;
};

class CircleBody : public Body {
public:
  using Config = CircleBodyConfig;
  b2Vec2 getCenter();
  float getRadius();
  const std::weak_ptr<Circle> getCircle();

protected:
  CircleBody() = delete;
  CircleBody(entt::registry &registry, const std::shared_ptr<World> world,
             const CircleBodyConfig &config,
             const std::shared_ptr<ShapeFactory> shapeFactory,
             const std::shared_ptr<JointFactory> jointFactory);
  std::weak_ptr<Circle> circle;

  friend class BodyFactory;
};