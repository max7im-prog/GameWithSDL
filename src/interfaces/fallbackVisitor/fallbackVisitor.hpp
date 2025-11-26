#pragma once
#include "visitor.hpp"

#include "body.hpp"
#include "capsule.hpp"
#include "capsuleBody.hpp"
#include "capsuleTerrain.hpp"
#include "circle.hpp"
#include "circleBody.hpp"
#include "circleTerrain.hpp"
#include "connection.hpp"
#include "creature.hpp"
#include "demoCreature.hpp"
#include "distanceConnection.hpp"
#include "distanceJoint.hpp"
#include "girdleConnection.hpp"
#include "joint.hpp"
#include "limbBody.hpp"
#include "mouseJoint.hpp"
#include "polygon.hpp"
#include "polygonBody.hpp"
#include "polygonTerrain.hpp"
#include "prismaticJoint.hpp"
#include "registryComposite.hpp"
#include "revoluteConnection.hpp"
#include "revoluteJoint.hpp"
#include "sceneNode.hpp"
#include "segment.hpp"
#include "segmentBody.hpp"
#include "segmentTerrain.hpp"
#include "shape.hpp"
#include "terrain.hpp"
#include "weldConnection.hpp"
#include "weldJoint.hpp"

/**
 * @brief A visitor implementation that routes the visit() method down the
 * heirarchy.
 *
 * Example: visit(DemoCreature) calls visit(Creature) calls
 * visit(ScencNode) calls visit(REgistryComposite).
 * It is designed to be a base class for other visitors so that the programmer
 * wouldn't need to override many similar looking visit() methods and would be
 * able to just override a visit() method for a base class.
 *
 */
class FallbackVisitor : public Visitor {
public:
  virtual ~FallbackVisitor() = 0;

  virtual void visit([[maybe_unused]] RegistryComposite *node) override;

  virtual void visit([[maybe_unused]] SceneNode *node) override;

  virtual void visit([[maybe_unused]] Shape *node) override;
  virtual void visit([[maybe_unused]] Circle *node) override;
  virtual void visit([[maybe_unused]] Segment *node) override;
  virtual void visit([[maybe_unused]] Capsule *node) override;
  virtual void visit([[maybe_unused]] Polygon *node) override;

  virtual void visit([[maybe_unused]] Joint *node) override;
  virtual void visit([[maybe_unused]] DistanceJoint *node) override;
  virtual void visit([[maybe_unused]] MouseJoint *node) override;
  virtual void visit([[maybe_unused]] PrismaticJoint *node) override;
  virtual void visit([[maybe_unused]] RevoluteJoint *node) override;
  virtual void visit([[maybe_unused]] WeldJoint *node) override;

  virtual void visit([[maybe_unused]] Body *node) override;
  virtual void visit([[maybe_unused]] CircleBody *node) override;
  virtual void visit([[maybe_unused]] SegmentBody *node) override;
  virtual void visit([[maybe_unused]] CapsuleBody *node) override;
  virtual void visit([[maybe_unused]] PolygonBody *node) override;
  virtual void visit([[maybe_unused]] LimbBody *node) override;

  virtual void visit([[maybe_unused]] Connection *node) override;
  virtual void visit([[maybe_unused]] DistanceConnection *node) override;
  virtual void visit([[maybe_unused]] GirdleConnection *node) override;
  virtual void visit([[maybe_unused]] RevoluteConnection *node) override;
  virtual void visit([[maybe_unused]] WeldConnection *node) override;

  virtual void visit([[maybe_unused]] Terrain *node) override;
  virtual void visit([[maybe_unused]] CapsuleTerrain *node) override;
  virtual void visit([[maybe_unused]] CircleTerrain *node) override;
  virtual void visit([[maybe_unused]] PolygonTerrain *node) override;
  virtual void visit([[maybe_unused]] SegmentTerrain *node) override;

  virtual void visit([[maybe_unused]] Creature *node) override;
  virtual void visit([[maybe_unused]] DemoCreature *node) override;

protected:
private:
};