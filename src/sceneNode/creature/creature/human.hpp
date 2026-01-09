
#pragma once
#include "bodyFactory.hpp"
#include "connectionFactory.hpp"
#include "creature.hpp"
#include <entt/entt.hpp>

struct HumanConfig : public CreatureConfig {
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;
  float sizeXMeters = 1;
  float sizeYMeters = 1;
};

class Human : public Creature, public VisitableImpl<Human> {
public:
  using Config = HumanConfig;

  void move(b2Vec2 dir) override;
  void lookAt(b2Vec2 worldPoint) override;
  virtual void update(float dt) override;
  virtual ~Human() = default;

  b2Vec2 getWorldPos() override;

  const b2Rot getRotation() const;

protected:
  Human(entt::registry &registry, const std::shared_ptr<World> world,
        const HumanConfig &config,
        const std::shared_ptr<BodyFactory> bodyFactory,
        const std::shared_ptr<ConnectionFactory> connectionFactory);

  std::weak_ptr<PolygonBody> _head;
  std::weak_ptr<CapsuleBody> _neck;
  std::weak_ptr<PolygonBody> _torso;
  std::weak_ptr<CircleBody> _shoulderLeft;
  std::weak_ptr<CircleBody> _shoulderRight;
  std::weak_ptr<CircleBody> _hipLeft;
  std::weak_ptr<CircleBody> _hipRight;
  std::weak_ptr<LimbBody> _armLeft;
  std::weak_ptr<LimbBody> _armRight;
  std::weak_ptr<LimbBody> _legLeft;
  std::weak_ptr<LimbBody> _legRight;

private:
  friend class CreatureFactory;
};
