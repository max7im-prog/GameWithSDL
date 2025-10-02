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

class SceneNode {
public:
  virtual ~SceneNode() = default;
  virtual void accept(Visitor &v) = 0;

protected:
private:
};