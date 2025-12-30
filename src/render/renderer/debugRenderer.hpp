#pragma once

#include "capsule.hpp"
#include "circle.hpp"
#include "mouseJoint.hpp"
#include "polygon.hpp"
#include "prismaticJoint.hpp"
#include "renderer.hpp"
#include "revoluteJoint.hpp"
#include "segment.hpp"
#include "weldJoint.hpp"

class DebugRenderer : public Renderer {
public:
  DebugRenderer(RenderContext &c);
  void visit(Circle &c) override;
  void visit(Polygon &p) override;
  void visit(Segment &s) override;
  void visit(Capsule &c) override;
  void visit(MouseJoint &) override;
  void visit(PrismaticJoint &) override;
  void visit(RevoluteJoint &) override;
  void visit(WeldJoint &) override;

  enum RenderColor {
    CIRCLE_COLOR,
    POLYGON_COLOR,
    SEGMENT_COLOR,
    CAPSULE_COLOR,
    JOINT_COLOR
  };

protected:
  SDL_Color getSDLColor(const RenderColor &c);
  static const std::map<RenderColor, SDL_Color> colorMapping;

private:
};
