#pragma once

#include "renderContext.hpp"
#include "visitor.hpp"
#include <entt/entt.hpp>
#include "fallbackVisitor.hpp"

class Renderer : public FallbackVisitor  {
public:
  Renderer(RenderContext &c);
  virtual ~Renderer() = 0;

protected:
  RenderContext &context;

private:
};