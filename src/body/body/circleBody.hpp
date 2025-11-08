#pragma once
#include "body.hpp"
#include "circle.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"

struct CircleBodyConfig : public BodyConfig {
  void defaultConfig() override;
  CircleConfig shapeCfg;
};

class CircleBody : public Body , public Visitable<CircleBody>{
public:
  using Config = CircleBodyConfig;
  b2Vec2 getCenter();
  float getRadius();
  const std::shared_ptr<Circle> getCircle();
  virtual b2Vec2 getWorldPos() override;

protected:
  CircleBody() = delete;
  CircleBody(entt::registry &registry, const std::shared_ptr<World> world,
             const CircleBodyConfig &config,
             const std::shared_ptr<ShapeFactory> shapeFactory,
             const std::shared_ptr<JointFactory> jointFactory);
  std::weak_ptr<Circle> circle;

  friend class BodyFactory;
};