#pragma once

#include <box2d/box2d.h>
#include <vector>

class KinematicUtils
{
public:
    // Solves an inverse kinematic problem with a FABRIK method
    // points - old locations of joints
    static std::vector<b2Vec2> solveFABRIK(b2Vec2 fixture, b2Vec2 target, const std::vector<b2Vec2> &points, float marginMtr = 0.01, size_t maxIter = 10);

    // Solves an inverse kinematic problem with a FABRIK method
    // This version of a function theoretically works better as it does not compute the length of limbs on each step
    // points - old locations of joints
    static std::vector<b2Vec2> solveFABRIK(b2Vec2 fixture, b2Vec2 target, const std::vector<b2Vec2> &points, const std::vector<float> &lengths, float marginMtr = 0.01, size_t maxIter = 10);

    // Returns angles in each joint of a sequence. ret[0] equals to the angle between x+ axis and a first segment. After that, values are angles between 2 consecutive segments
    static std::vector<float> getAngles(const std::vector<b2Vec2> &pos);

private:
};