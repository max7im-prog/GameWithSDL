#pragma once
#include "shape.hpp"

struct PolygonConfig : public ShapeConfig {
  static PolygonConfig defaultConfig();
  std::vector<b2Vec2> vertices;
  float radius;
};

class Polygon : public Shape , public Visitable<Polygon>{
public:
  using Config = PolygonConfig;
  virtual ~Polygon() = default;

  std::vector<b2Vec2> getVertices() const;

protected:
  Polygon() = delete;
  Polygon(entt::registry &registry, const World &world,
          const PolygonConfig &config);

  friend class ShapeFactory;
};