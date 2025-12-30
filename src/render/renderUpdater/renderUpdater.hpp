#pragma once

#include "demoCreature.hpp"
#include "fallbackVisitor.hpp"
class RenderUpdater : public FallbackVisitor {
public:
  virtual void visit(Shape *shape) override;
  virtual void visit(Body *body) override;
  virtual void visit(Creature *creature) override;

  virtual void visit(DemoCreature *creature) override;

protected:
private:
};
