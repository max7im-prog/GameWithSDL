#include "debugRenderer.hpp"

#include "renderUtils.hpp"

const std::map<DebugRenderer::RenderColor, SDL_Color>
    DebugRenderer::colorMapping = {
        {DebugRenderer::RenderColor::CAPSULE_COLOR, {255, 0, 0, 255}},
        {DebugRenderer::RenderColor::CIRCLE_COLOR, {0, 255, 0, 255}},
        {DebugRenderer::RenderColor::SEGMENT_COLOR, {0, 0, 255, 255}},
        {DebugRenderer::RenderColor::POLYGON_COLOR, {255, 0, 255, 255}},
        {DebugRenderer::RenderColor::SEGMENT_COLOR, {255, 0, 255, 255}}};

DebugRenderer::DebugRenderer(RenderContext &c) : Renderer(c) {}

void DebugRenderer::visit(Circle *c) {
  auto center = RenderUtils::b2VecToScreenPos(c->getCenter(), _renderContext);
  auto radius = c->getRadius() * _renderContext.getPixelToMeterRatio();
  RenderUtils::renderCircle(center, radius,
                            getSDLColor(RenderColor::CIRCLE_COLOR), _renderContext);
}

void DebugRenderer::visit(Polygon *p) {
  auto vertices = p->getVertices();
  std::vector<ScreenPos> screenPoints;
  screenPoints.resize(vertices.size());
  std::transform(vertices.begin(), vertices.end(), screenPoints.begin(),
                 [&](const b2Vec2 &pos) -> ScreenPos {
                   return RenderUtils::b2VecToScreenPos(pos, _renderContext);
                 });
  RenderUtils::renderPolygon(screenPoints,
                             getSDLColor(RenderColor::POLYGON_COLOR), _renderContext);
}

void DebugRenderer::visit(Segment *s) {
  auto p1 = RenderUtils::b2VecToScreenPos(s->getPoint1(), _renderContext);
  auto p2 = RenderUtils::b2VecToScreenPos(s->getPoint2(), _renderContext);
  RenderUtils::renderSegment(p1, p2, getSDLColor(RenderColor::SEGMENT_COLOR),
                             _renderContext);
}

void DebugRenderer::visit(Capsule *c) {
  auto p1 = RenderUtils::b2VecToScreenPos(c->getCenter1(), _renderContext);
  auto p2 = RenderUtils::b2VecToScreenPos(c->getCenter2(), _renderContext);
  auto radius = c->getRadius() * _renderContext.getPixelToMeterRatio();
  RenderUtils::renderCapsule(p1, p2, radius,
                             getSDLColor(RenderColor::CAPSULE_COLOR), _renderContext);
}

void DebugRenderer::visit(MouseJoint *) {}

void DebugRenderer::visit(PrismaticJoint *) {}

void DebugRenderer::visit(RevoluteJoint *) {}

void DebugRenderer::visit(WeldJoint *) {}

SDL_Color DebugRenderer::getSDLColor(const RenderColor &c) {
  return DebugRenderer::colorMapping.at(c);
}
