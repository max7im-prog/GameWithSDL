#include "renderer.hpp"
#include "renderContext.hpp"

Renderer::Renderer(entt::registry &r, RenderContext &c)
    : context(c), registry(r) {}

Renderer::~Renderer() = default;