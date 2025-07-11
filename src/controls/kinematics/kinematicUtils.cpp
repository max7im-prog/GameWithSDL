#include "kinematicUtils.hpp"
#include <numeric>
#include <cmath>

std::vector<b2Vec2> KinematicUtils::solveFABRIK(
    b2Vec2 fixture,
    b2Vec2 target,
    const std::vector<b2Vec2> &points,
    float marginMtr,
    size_t maxIter)
{
    std::vector<b2Vec2> ret = points;
    if (points.size() < 2)
        return ret;

    int numPoints = points.size();
    std::vector<float> lengths(numPoints - 1);
    for (int i = 0; i < numPoints - 1; i++)
    {
        lengths[i] = b2Distance(points[i], points[i + 1]);
    }

    return KinematicUtils::solveFABRIK(fixture,target,points,lengths,marginMtr,maxIter);
}

std::vector<b2Vec2> KinematicUtils::solveFABRIK(
    b2Vec2 fixture,
    b2Vec2 target,
    const std::vector<b2Vec2> &points,
    const std::vector<float> &lengths,
    float marginMtr,
    size_t maxIter)
{
    std::vector<b2Vec2> ret = points; 
    size_t numPoints = lengths.size() +1;

    float totalDist = 0;
    for (auto &elem : lengths)
    {
        totalDist += elem;
    }

    if (totalDist < b2Distance(target, fixture))
    {
        // Target unreachable - point straight towards target
        b2Vec2 dir = b2Normalize(b2Sub(target,fixture));
        ret[0] = fixture;
        for(size_t i = 1;i<numPoints;i++){
            ret[i] = b2Add(ret[i-1],b2MulSV(lengths[i-1],dir));
        }
    }
    else
    {
        float curMargin = b2Distance(ret.back(), target);
        for (int iter = 0; iter < maxIter && curMargin > marginMtr; iter++)
        {
            // BACKWARD
            ret.back() = target;
            for (int i = numPoints - 2; i >= 0; i--)
            {
                b2Vec2 dir = b2Normalize(b2Sub(ret[i], ret[i + 1]));
                ret[i] = b2Add(ret[i + 1], b2MulSV(lengths[i], dir));
            }

            // FORWARD
            ret[0] = fixture;
            for (int i = 1; i < numPoints; i++)
            {
                b2Vec2 dir = b2Normalize(b2Sub(ret[i], ret[i - 1]));
                ret[i] = b2Add(ret[i - 1], b2MulSV(lengths[i - 1], dir));
            }

            curMargin = b2Distance(ret.back(), target);
        }
    }

    return ret;
}

std::vector<float> KinematicUtils::getAngles(const std::vector<b2Vec2> &pos)
{
    if (pos.size() < 2)
        return {};

    std::vector<float> ret;
    b2Vec2 lastVec = {1, 0};  // Reference direction (e.g., pointing along X axis)

    for (size_t i = 0; i < pos.size() - 1; ++i)
    {
        b2Vec2 curVec = b2Sub(pos[i + 1], pos[i]);

        float lastAngle = b2Atan2(lastVec.y, lastVec.x);
        float curAngle = b2Atan2(curVec.y, curVec.x);
        float angleDiff = curAngle - lastAngle;

        // Normalize angle to [-π, π]
        angleDiff = std::fmod(angleDiff + M_PI, 2.0f * M_PI);
        if (angleDiff < 0)
            angleDiff += 2.0f * M_PI;
        angleDiff -= M_PI;

        ret.push_back(angleDiff);

        lastVec = curVec;
    }

    return ret;
}

