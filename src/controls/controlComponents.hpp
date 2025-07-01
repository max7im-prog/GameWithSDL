#pragma once
#include <box2d/box2d.h>

struct PlayerControlled
{
};

struct Controller{
    b2Vec2 moveDirection {0,0};
    b2Vec2 aimWorldPoint {0,0};
    bool jump = false;
    bool attack = false;
    bool defend = false;
};