#pragma once
#include "bodyPart.hpp"
#include "circle.hpp"

struct CircleBodyConfig : public BodyPartConfig {
  static CircleBodyConfig defaultConfig();
  CircleConfig circleConfig;
};

class CircleBody : public BodyPart {
public:
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