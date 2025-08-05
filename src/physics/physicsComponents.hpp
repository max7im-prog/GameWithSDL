#pragma once
#include "joint.hpp"
#include "shape.hpp"
#include "world.hpp"
#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include <variant>
#include <vector>

typedef std::variant<b2Polygon, b2Circle, b2Capsule, b2Segment> ShapeVariant;

struct PhysicsWorld {
  std::shared_ptr<World> world;

  // TODO: delete
  b2WorldId worldId;
};

struct PhysicsBody {
  std::shared_ptr<Shape> shape;

  // TODO: delete
  b2BodyId bodyId;
  b2WorldId worldId;
  std::vector<b2ShapeId> shapes;
};

struct PhysicsJoint {
  std::shared_ptr<Joint> joint;

  // TODO: delete
  b2JointId jointId;
  b2BodyId bodyAId;
  b2BodyId bodyBId;
  b2WorldId worldId;
};

// TODO: delete
struct PendingPhysicsBody {
  b2BodyDef bodyDef;
  b2WorldId worldId;
  std::vector<std::pair<b2ShapeDef, ShapeVariant>> shapeDefs;
};
