#pragma once
#include "bodyPart.hpp"
#include "capsule.hpp"

struct CapsuleBodyConfig : public BodyPartConfig {
  static CapsuleBodyConfig defaultConfig();
  CapsuleConfig capsuleConfig;
};

class CapsuleBody : public BodyPart {
public:
  b2Vec2 getCenter1();
  b2Vec2 getCenter2();
  float getRadius();
  const std::shared_ptr<Capsule> getCapsule();

protected:
  CapsuleBody() = delete;
  CapsuleBody(entt::registry &registry, const std::shared_ptr<World> world,
             const CapsuleBodyConfig &config,
             const std::shared_ptr<PhysicsFactory> physicsFactory);
  std::shared_ptr<Capsule> capsule;

  friend class BodyFactory;
};