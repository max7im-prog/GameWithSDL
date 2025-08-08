#pragma once
#include "bodyPart.hpp"
#include "creature/creature/creature.hpp"
#include "joint.hpp"
#include "shape.hpp"
#include "world.hpp"
#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include <variant>

typedef std::variant<b2Polygon, b2Circle, b2Capsule, b2Segment> ShapeVariant;

struct PhysicsWorld {
  std::shared_ptr<World> world;
};

struct PhysicsBody {
  std::shared_ptr<Shape> shape;
};

struct PhysicsJoint {
  std::shared_ptr<Joint> joint;
};

struct PhysicsBodyPart {
  std::shared_ptr<BodyPart> bodyPart;
};

struct PhysicsCreature{
  std::shared_ptr<Creature> creature;
};
