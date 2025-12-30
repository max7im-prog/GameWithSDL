#pragma once

#include "demoCreature.hpp"
#include "entt/entity/fwd.hpp"
#include "fallbackVisitor.hpp"
class RenderUpdater : public FallbackVisitor {
public:
  RenderUpdater(entt::registry& registry);

  virtual void visit(Shape &shape) override;
  virtual void visit(Body &body) override;
  virtual void visit(Creature &creature) override;

  virtual void visit(DemoCreature &creature) override;

protected:
private:
  entt::registry& _registry;
};
