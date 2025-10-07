#include "renderer.hpp"
#include "renderContext.hpp"

Renderer::Renderer(RenderContext &c) : context(c) {}

Renderer::~Renderer() = default;