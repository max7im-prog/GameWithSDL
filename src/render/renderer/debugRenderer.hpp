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
  DebugRenderer( RenderContext &c);
  void visit(Circle *c);
  void visit(Polygon *p);
  void visit(Segment *s);
  void visit(Capsule *c);
  void visit(MouseJoint *);
  void visit(PrismaticJoint *);
  void visit(RevoluteJoint *);
  void visit(WeldJoint *);

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