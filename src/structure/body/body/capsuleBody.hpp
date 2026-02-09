#pragma once
#include "body.hpp"
#include "capsule.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"

class CapsuleBody : public Body, public VisitableImpl<CapsuleBody> {
public:
  struct Config : public Body::Config {
    void defaultConfig() override;
    Capsule::Config shapeCfg;
  };
  struct ShapeNames {
    static constexpr std::string_view s_main = "main";

  private:
    ~ShapeNames() = delete;
  };
  b2Vec2 getCenter1();
  b2Vec2 getCenter2();
  float getRadius();
  const std::shared_ptr<Capsule> getCapsule();
  virtual b2Vec2 getWorldPos() override;

protected:
  CapsuleBody() = delete;
  CapsuleBody(entt::registry &registry, const std::shared_ptr<World> world,
              const CapsuleBody::Config &config,
              const std::shared_ptr<ShapeFactory> shapeFactory,
              const std::shared_ptr<JointFactory> jointFactory);
  std::weak_ptr<Capsule> capsule;

  friend class BodyFactory;
};
