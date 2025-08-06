#pragma once
#include "shape.hpp"

struct PolygonConfig : public ShapeConfig {
  static PolygonConfig defaultConfig();
  std::vector<b2Vec2> vertices;
  float radius;
};

class Polygon : public Shape {
public:
  virtual ~Polygon() = default;

protected:
  Polygon() = delete;
  Polygon(entt::registry& registry,const World &world, const PolygonConfig &config);

  friend class PhysicsFactory;
};