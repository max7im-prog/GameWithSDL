#pragma once
#include "baseCreature.hpp"

struct Creature{
    std::unique_ptr<BaseCreature> creature;
};