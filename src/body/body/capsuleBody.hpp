#pragma once
#include "body.hpp"
#include "capsule.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"

struct CapsuleBodyConfig : public BodyConfig {
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;
  CapsuleConfig shapeCfg;
};

class CapsuleBody : public Body , public Visitable<CapsuleBody>{
public:
  using Config = CapsuleBodyConfig;
  b2Vec2 getCenter1();
  b2Vec2 getCenter2();
  float getRadius();
  const std::shared_ptr<Capsule> getCapsule();

protected:
  CapsuleBody() = delete;
  CapsuleBody(entt::registry &registry, const std::shared_ptr<World> world,
              const CapsuleBodyConfig &config,
              const std::shared_ptr<ShapeFactory> shapeFactory,
              const std::shared_ptr<JointFactory> jointFactory);
  std::weak_ptr<Capsule> capsule;

  friend class BodyFactory;
};