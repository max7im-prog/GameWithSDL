#pragma once
#include "box2d/box2d.h"
#include "joint.hpp"
#include "objectConfig.hpp"
#include "registryComposite.hpp"
#include "shape.hpp"

struct BodyConfig :public ObjectConfig{};

class Body : public RegistryComposite, public virtual IVisitable {
public:
  virtual void update([[maybe_unused]] float dt) override {};
  virtual ~Body();
  virtual b2Vec2 getWorldPos() = 0;

protected:
  Body(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> world;

private:
  friend class BodyFactory;
};