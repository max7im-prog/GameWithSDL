#pragma once

class Joint;
class Body;
class Connection;
class Creature;
class Terrain;

class Shape;
class Circle;
class Segment;
class Capsule;
class Polygon;

class Visitor {
public:
  virtual ~Visitor() = default;
  virtual void visit([[maybe_unused]] Shape *) {}
  virtual void visit([[maybe_unused]] Circle *) {}
  virtual void visit([[maybe_unused]] Segment *) {}
  virtual void visit([[maybe_unused]] Capsule *) {}
  virtual void visit([[maybe_unused]] Polygon *) {}
  
  virtual void visit([[maybe_unused]] Joint *) {}
  virtual void visit([[maybe_unused]] Body *) {}
  virtual void visit([[maybe_unused]] Connection *) {}
  virtual void visit([[maybe_unused]] Creature *) {}
  virtual void visit([[maybe_unused]] Terrain *) {}

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
  void accept(Visitor &v) override { v.visit(static_cast<Derived *>(this)); }
};