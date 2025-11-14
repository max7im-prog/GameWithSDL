#pragma once

#include "registryComposite.hpp"
class RegistryComposite;
class Shape;
class Circle;
class Segment;
class Capsule;
class Polygon;

class Joint;
class DistanceJoint;
class MouseJoint;
class PrismaticJoint;
class RevoluteJoint;
class WeldJoint;

class Body;
class CircleBody;
class SegmentBody;
class CapsuleBody;
class PolygonBody;
class LimbBody;

class Connection;
class DistanceConnection;
class GirdleConnection;
class RevoluteConnection;
class WeldConnection;

class Terrain;
class CapsuleTerrain;
class CircleTerrain;
class PolygonTerrain;
class SegmentTerrain;

class Creature;
class DemoCreature;

class Visitor {
public:
  virtual ~Visitor() = 0;
  virtual void visit([[maybe_unused]] RegistryComposite *) {}
  virtual void visit([[maybe_unused]] Shape *) {}
  virtual void visit([[maybe_unused]] Circle *) {}
  virtual void visit([[maybe_unused]] Segment *) {}
  virtual void visit([[maybe_unused]] Capsule *) {}
  virtual void visit([[maybe_unused]] Polygon *) {}

  virtual void visit([[maybe_unused]] Joint *) {}
  virtual void visit([[maybe_unused]] DistanceJoint *) {}
  virtual void visit([[maybe_unused]] MouseJoint *) {}
  virtual void visit([[maybe_unused]] PrismaticJoint *) {}
  virtual void visit([[maybe_unused]] RevoluteJoint *) {}
  virtual void visit([[maybe_unused]] WeldJoint *) {}

  virtual void visit([[maybe_unused]] Body *) {}
  virtual void visit([[maybe_unused]] CircleBody *) {}
  virtual void visit([[maybe_unused]] SegmentBody *) {}
  virtual void visit([[maybe_unused]] CapsuleBody *) {}
  virtual void visit([[maybe_unused]] PolygonBody *) {}
  virtual void visit([[maybe_unused]] LimbBody *) {}

  virtual void visit([[maybe_unused]] Connection *) {}
  virtual void visit([[maybe_unused]] DistanceConnection *) {}
  virtual void visit([[maybe_unused]] GirdleConnection *) {}
  virtual void visit([[maybe_unused]] RevoluteConnection *) {}
  virtual void visit([[maybe_unused]] WeldConnection *) {}

  virtual void visit([[maybe_unused]] Terrain *) {}
  virtual void visit([[maybe_unused]] CapsuleTerrain *) {}
  virtual void visit([[maybe_unused]] CircleTerrain *) {}
  virtual void visit([[maybe_unused]] PolygonTerrain *) {}
  virtual void visit([[maybe_unused]] SegmentTerrain *) {}

  virtual void visit([[maybe_unused]] Creature *) {}
  virtual void visit([[maybe_unused]] DemoCreature *) {}

protected:
private:
};

class IVisitable {
public:
  virtual ~IVisitable() = default;
  virtual void accept(Visitor &v) = 0;
};

/**
 * @brief An interface for objects that can be visited by Visitor
 *
 * @tparam Derived Interface implements an accept function for an object. This
 * argument shows how the visitor should interpret an object in an accept()
 * function. Solves the CRTP problem.
 */
template <typename Derived> class Visitable : public virtual IVisitable {
public:
  virtual void accept(Visitor &v) override {
    v.visit(static_cast<Derived *>(this));
  }
};