#pragma once
#include "shape.hpp"

class Circle : public Shape, public VisitableImpl<Circle> {
public:
  struct Config : public Shape::Config {
    void defaultConfig() override;
    float radius;
  };
  virtual ~Circle() = default;
  b2Vec2 getCenter();
  b2Vec2 getLocalCenter();
  float getRadius();

protected:
  Circle() = delete;
  Circle(entt::registry &registry, const World &world,
         const Circle::Config &config);

  friend class ShapeFactory;
};
