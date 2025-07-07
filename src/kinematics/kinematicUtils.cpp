#include "kinematicUtils.hpp"

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

    return ret;
}
