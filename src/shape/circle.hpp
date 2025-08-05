#pragma once
#include "shape.hpp"

struct CircleConfig : public ShapeConfig {
  CircleConfig defaultConfig();
  float radius;
};

class Circle : public Shape {
public:
  virtual ~Circle() = default;

protected:
  Circle() = delete;
  Circle(const World &world, const CircleConfig &config);

  friend class PhysicsFactory;
};