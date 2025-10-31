#pragma once
#include "box2d/math_functions.h"
#include "shape.hpp"

struct SegmentConfig : public ShapeConfig {
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;
  b2Vec2 localPoint1, localPoint2;
};

class Segment : public Shape, public Visitable<Segment> {
public:
  using Config = SegmentConfig;
  virtual ~Segment() = default;
  b2Vec2 getPoint1();
  b2Vec2 getPoint2();

protected:
  Segment() = delete;
  Segment(entt::registry &registry, const World &world,
          const SegmentConfig &config);

  friend class ShapeFactory;
};