#pragma once
#include "shape.hpp"

struct CapsuleConfig : public ShapeConfig {
  static CapsuleConfig defaultConfig();
  float radius;
  b2Vec2 center1;
  b2Vec2 center2;
};

class Capsule : public Shape {
public:
  virtual ~Capsule() = default;

protected:
  Capsule() = delete;
  Capsule(entt::registry &registry, const World &world,
          const CapsuleConfig &config);

  friend class PhysicsFactory;
};