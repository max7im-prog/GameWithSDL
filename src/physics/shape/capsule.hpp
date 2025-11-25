#pragma once
#include "shape.hpp"

struct CapsuleConfig : public ShapeConfig {
  void defaultConfig() override;
  float radius;
  b2Vec2 center1;
  b2Vec2 center2;
};

class Capsule : public Shape , public VisitableImpl<Capsule>{
public:
  using Config = CapsuleConfig;
  virtual ~Capsule() = default;
  b2Vec2 getCenter1();
  b2Vec2 getCenter2();
  b2Vec2 getLocalCenter1();
  b2Vec2 getLocalCenter2();
  float getRadius();

protected:
  Capsule() = delete;
  Capsule(entt::registry &registry, const World &world,
          const CapsuleConfig &config);

  friend class ShapeFactory;
};