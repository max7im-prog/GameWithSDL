#pragma once

#include "renderContext.hpp"
#include "visitor.hpp"
#include <entt/entt.hpp>
class Renderer : public Visitor {
public:
  Renderer(RenderContext &c);
  virtual ~Renderer() = 0;

protected:
  RenderContext &context;

private:
};