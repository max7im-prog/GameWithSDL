#pragma once
#include "box2d/box2d.h"
#include "joint.hpp"
#include "registryComposite.hpp"
#include "shape.hpp"

struct BodyConfig {};

class Body : public RegistryComposite ,public SceneNode{
public:
  virtual void update([[maybe_unused]] float dt) override {};
  virtual ~Body();

  virtual void accept(Visitor &v) override;
protected:
  Body(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> world;

private:
  friend class BodyFactory;
};