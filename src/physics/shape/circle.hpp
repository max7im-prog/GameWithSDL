#pragma once
#include "shape.hpp"

struct CircleConfig : public ShapeConfig {
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;
  float radius;
};

class Circle : public Shape, public Visitable<Circle> {
public:
  using Config = CircleConfig;
  virtual ~Circle() = default;
  b2Vec2 getCenter();
  b2Vec2 getLocalCenter();
  float getRadius();

protected:
  Circle() = delete;
  Circle(entt::registry &registry, const World &world,
         const CircleConfig &config);

  friend class ShapeFactory;
};