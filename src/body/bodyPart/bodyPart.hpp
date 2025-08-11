#pragma once
#include "box2d/box2d.h"
#include "joint.hpp"
#include "physicsFactory.hpp"
#include "registryObject.hpp"
#include "shape.hpp"

struct BodyPartConfig {};

class BodyPart : public RegistryObject {
public:
  virtual void update([[maybe_unused]] float dt) {};
  virtual ~BodyPart();

protected:
  BodyPart(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> world;

private:
  BodyPart() = delete;
  BodyPart(BodyPart &other) = delete;
  BodyPart(BodyPart &&other) = delete;
  BodyPart operator=(BodyPart &other) = delete;
  BodyPart operator=(BodyPart &&other) = delete;
  friend class BodyFactory;
};