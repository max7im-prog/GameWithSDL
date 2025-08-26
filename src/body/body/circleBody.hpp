#pragma once
#include "body.hpp"
#include "circle.hpp"

struct CircleBodyConfig : public BodyConfig {
  static CircleBodyConfig defaultConfig();
  CircleConfig shapeCfg;
};

class CircleBody : public Body {
public:
  using Config = CircleBodyConfig;
  b2Vec2 getCenter();
  float getRadius();
  const std::shared_ptr<Circle> getCircle();

protected:
  CircleBody() = delete;
  CircleBody(entt::registry &registry, const std::shared_ptr<World> world,
             const CircleBodyConfig &config,
             const std::shared_ptr<PhysicsFactory> physicsFactory);
  std::shared_ptr<Circle> circle;

  friend class BodyFactory;
};