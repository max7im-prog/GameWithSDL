#pragma once
#include "shape.hpp"

struct CircleConfig : public ShapeConfig {
  CircleConfig defaultCircleConfig();
  float radius;
};

class Circle : public Shape {
protected:
  Circle() = delete;
  Circle(const World &world,
         const CircleConfig &config);

  friend class PhysicsFactory;
};