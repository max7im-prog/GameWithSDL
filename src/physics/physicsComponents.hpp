#pragma once
#include <box2d/box2d.h>
#include <variant>
#include <SDL3/SDL.h>
#include <vector>
#include "world.hpp"
#include "shape.hpp"

typedef std::variant<b2Polygon, b2Circle, b2Capsule, b2Segment> ShapeVariant ;

struct PhysicsWorld
{
    std::shared_ptr<World> world;

    // TODO: delete
    b2WorldId worldId;
};

struct PhysicsBody
{
    std::shared_ptr<Shape> shape;

    // TODO: delete
    b2BodyId bodyId;
    b2WorldId worldId;
    std::vector<b2ShapeId> shapes;
};

struct PhysicsJoint
{
    // TODO: delete
    b2JointId jointId;
    b2BodyId bodyAId;
    b2BodyId bodyBId;
    b2WorldId worldId;
};

struct PendingPhysicsBody
{
    b2BodyDef bodyDef;
    b2WorldId worldId;
    std::vector<std::pair<
        b2ShapeDef, ShapeVariant>>
        shapeDefs;
};
