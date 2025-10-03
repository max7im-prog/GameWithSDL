#pragma once

class Shape;
class Joint;
class Body;
class Connection;
class Creature;
class Terrain;

class Visitor {
public:
  virtual ~Visitor() = default;
  virtual void visit([[maybe_unused]] Shape *shape) {}
  virtual void visit([[maybe_unused]] Joint *joint) {}
  virtual void visit([[maybe_unused]] Body *body) {}
  virtual void visit([[maybe_unused]] Connection *connection) {}
  virtual void visit([[maybe_unused]] Creature *creature) {}
  virtual void visit([[maybe_unused]] Terrain *terrain) {}

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
template <typename Derived> class Visitable : public IVisitable {
public:
  void accept(Visitor &v) override { v.visit(static_cast<Derived *>(this)); }
};