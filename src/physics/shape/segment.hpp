#pragma once
#include "box2d/math_functions.h"
#include "shape.hpp"

class Segment : public Shape, public VisitableImpl<Segment> {
public:
  struct Config : public Shape::Config {
    void defaultConfig() override;
    b2Vec2 localPoint1, localPoint2;
  };
  virtual ~Segment() = default;
  b2Vec2 getPoint1();
  b2Vec2 getPoint2();

protected:
  Segment() = delete;
  Segment(entt::registry &registry, const World &world,
          const Segment::Config &config);

  friend class ShapeFactory;
};
