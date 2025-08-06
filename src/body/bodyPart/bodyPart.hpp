#pragma once
#include "box2d/box2d.h"
#include "joint.hpp"
#include "physicsFactory.hpp"
#include "shape.hpp"

struct BodyPartConfig {
};

class BodyPart {
public:
  virtual void update([[maybe_unused]] float dt) {};
  entt::entity getEntity();
  virtual ~BodyPart();

protected:
  BodyPart(entt::registry &registry, const std::shared_ptr<World> world);
  BodyPart() = delete;
  BodyPart(BodyPart &other) = delete;
  BodyPart(BodyPart &&other) = delete;
  BodyPart operator=(BodyPart &other) = delete;
  BodyPart operator=(BodyPart &&other) = delete;

  const std::shared_ptr<World> world;

private:
  entt::entity entity = entt::null;
  entt::registry &registry;
  friend class BodyFactory;
};