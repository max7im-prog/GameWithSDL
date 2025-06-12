#pragma once
#include <box2d/box2d.h>
#include <optional>
#include <variant>
#include <SDL3/SDL.h>

struct PhysicsWorld
{
    b2WorldId worldId;
};

struct PhysicsBody
{
    b2BodyId bodyId;
    b2WorldId worldId;
    std::vector<b2ShapeId> shapes;
    std::optional<SDL_Texture *> texture;
};

struct PendingPhysicsBody
{
    b2BodyDef bodyDef;
    b2WorldId worldId;
    std::vector<std::pair<
        b2ShapeDef, std::variant<b2Polygon, b2Circle, b2Capsule, b2Segment>>>
        shapeDefs;
    std::optional<SDL_Texture *> texture;
};
