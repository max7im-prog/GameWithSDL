#pragma once
#include "visitor.hpp"

#include "sceneNode.hpp"
#include "registryComposite.hpp"
#include "shape.hpp"
#include "circle.hpp"
#include "segment.hpp"
#include "capsule.hpp"
#include "polygon.hpp"
#include "joint.hpp"
#include "distanceJoint.hpp"
#include "mouseJoint.hpp"
#include "prismaticJoint.hpp"
#include "revoluteJoint.hpp"
#include "weldJoint.hpp"
#include "body.hpp"
#include "circleBody.hpp"
#include "segmentBody.hpp"
#include "capsuleBody.hpp"
#include "polygonBody.hpp"
#include "limbBody.hpp"
#include "connection.hpp"
#include "distanceConnection.hpp"
#include "girdleConnection.hpp"
#include "revoluteConnection.hpp"
#include "weldConnection.hpp"
#include "terrain.hpp"
#include "capsuleTerrain.hpp"
#include "circleTerrain.hpp"
#include "polygonTerrain.hpp"
#include "segmentTerrain.hpp"
#include "creature.hpp"
#include "demoCreature.hpp"




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