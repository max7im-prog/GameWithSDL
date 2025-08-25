#pragma once
#include "box2d/box2d.h"
#include "joint.hpp"
#include "physicsFactory.hpp"
#include "registryObject.hpp"
#include "shape.hpp"

struct BodyConfig {};

class Body : public RegistryObject {
public:
  virtual void update([[maybe_unused]] float dt) override {};
  virtual ~Body();

protected:
  Body(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> world;

private:
  friend class BodyFactory;
};