#pragma once

#include <box2d/box2d.h>
#include <vector>

class KinematicUtils{
public:
    // Solves an inverse kinematic problem with a FABRIK method
    // points - old locations of joints
    static std::vector<b2Vec2> solveFABRIK(b2Vec2 fixture, b2Vec2 target, const std::vector<b2Vec2> &points,float marginMtr = 0.01, size_t maxIter = 10);
private:

};