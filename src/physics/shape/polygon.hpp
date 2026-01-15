#pragma once
#include "shape.hpp"

class Polygon : public Shape, public VisitableImpl<Polygon> {
public:
  struct Config : public Shape::Config {
    void defaultConfig() override;
    std::vector<b2Vec2> vertices;
    float radius;
  };
  virtual ~Polygon() = default;

  std::vector<b2Vec2> getVertices() const;

protected:
  Polygon() = delete;
  Polygon(entt::registry &registry, const World &world,
          const Polygon::Config &config);

  friend class ShapeFactory;
};
