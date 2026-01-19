#pragma once
#include "shape.hpp"

class Capsule : public Shape, public VisitableImpl<Capsule> {
public:
  struct Config : public Shape::Config {
    void defaultConfig() override;
    float radius;
    b2Vec2 center1;
    b2Vec2 center2;
  };
  virtual ~Capsule() = default;
  b2Vec2 getCenter1();
  b2Vec2 getCenter2();
  b2Vec2 getLocalCenter1();
  b2Vec2 getLocalCenter2();
  float getRadius();

protected:
  Capsule() = delete;
  Capsule(entt::registry &registry, const World &world,
          const Capsule::Config &config);

  friend class ShapeFactory;
};
