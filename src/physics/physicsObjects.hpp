#pragma once
#include <box2d/box2d.h>

struct PhysicsWorld {
    b2WorldId worldId;
};

struct RigidBody {
    b2BodyId bodyId;
    b2WorldId wordlId;
};
