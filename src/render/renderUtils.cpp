#include "renderUtils.hpp"
#include "box2d/box2d.h"
#include "box2d/id.h"

#include <vector>

std::pair<int, int> RenderUtils::metersToPixels(std::pair<float, float> meters, const RenderContext &context)
{
    std::pair<float, float> ret = {0, 0}; // first - x, second - y
    ret.first = (meters.first - context.x) * context.pixelToMeterRatio;

    float topMeters = (context.y + context.h / context.pixelToMeterRatio);
    ret.second = (topMeters - meters.second) * context.pixelToMeterRatio;

    return ret;
}

std::pair<float, float> RenderUtils::pixelsToMeters(std::pair<int, int> pixels, const RenderContext &context)
{
    std::pair<float, float> ret = {0, 0}; // first - x, second - y
    ret.first = context.x + pixels.first / context.pixelToMeterRatio;
    ret.second = context.y + context.h / context.pixelToMeterRatio - pixels.second / context.pixelToMeterRatio;
    return ret;
}

bool RenderUtils::renderShape(b2ShapeId shapeId, const RenderContext &context)
{
    if(!b2Shape_IsValid(shapeId)){
        return false;
    }
    auto type = b2Shape_GetType(shapeId);
    auto bodyId = b2Shape_GetBody(shapeId);

    if (type == b2_circleShape)
    {
        auto circle = b2Shape_GetCircle(shapeId);
        auto transform = b2Body_GetTransform(bodyId);
        b2Vec2 worldCenter = b2TransformPoint(transform, circle.center);

        std::pair<int, int> pixelCenter = metersToPixels({worldCenter.x, worldCenter.y}, context);
        int pixelRadius = circle.radius * context.pixelToMeterRatio;

        renderCircle(pixelCenter, pixelRadius, {255, 0, 0, 255}, context);
    }
    else if (type == b2_capsuleShape)
    {
        auto capsule = b2Shape_GetCapsule(shapeId);
        auto transform = b2Body_GetTransform(bodyId);
        b2Vec2 worldPoint1 = b2TransformPoint(transform, capsule.center1);
        b2Vec2 worldPoint2 = b2TransformPoint(transform, capsule.center2);
        auto pixelPoint1 = metersToPixels({worldPoint1.x, worldPoint1.y}, context);
        auto pixelPoint2 = metersToPixels({worldPoint2.x, worldPoint2.y}, context);
        float pixelRadius = capsule.radius * context.pixelToMeterRatio;
        renderCapsule(pixelPoint1, pixelPoint2, pixelRadius, {255, 0, 255, 255}, context);
    }
    else if (type == b2_segmentShape)
    {
        auto segment = b2Shape_GetSegment(shapeId);
        auto transform = b2Body_GetTransform(bodyId);
        b2Vec2 worldPoint1 = b2TransformPoint(transform, segment.point1);
        b2Vec2 worldPoint2 = b2TransformPoint(transform, segment.point2);
        auto pixelPoint1 = metersToPixels({worldPoint1.x, worldPoint1.y}, context);
        auto pixelPoint2 = metersToPixels({worldPoint2.x, worldPoint2.y}, context);
        renderSegment(pixelPoint1, pixelPoint2, {0, 255, 0, 255}, context);
    }
    else if (type == b2_polygonShape)
    {
        auto polygon = b2Shape_GetPolygon(shapeId);
        auto transform = b2Body_GetTransform(bodyId);
        std::vector<std::pair<int, int>> points = {};
        for (int i = 0; i < polygon.count; i++)
        {
            b2Vec2 worldPoint = b2TransformPoint(transform, polygon.vertices[i]);
            auto pixelPoint = metersToPixels({worldPoint.x, worldPoint.y}, context);
            points.push_back(pixelPoint);
        }
        renderPolygon(points, {0, 0, 255, 255}, context);
    }
    else
    {

        return false;
    }

    return true;
}

void RenderUtils::renderCircle(std::pair<int, int> pixelCenter, int pixelRadius, SDL_Color color, const RenderContext &context)
{

    SDL_SetRenderDrawColor(context.renderer, color.r, color.g, color.b, color.a);
    const int sides = 16;
    const double angleStep = 2.0 * M_PI / sides;

    int cx = pixelCenter.first;
    int cy = pixelCenter.second;

    SDL_FPoint points[sides + 1]; // SDL_FPoint is preferred in SDL3 for subpixel rendering

    for (int i = 0; i <= sides; ++i)
    {
        double angle = i * angleStep;
        float x = static_cast<float>(cx + pixelRadius * std::cos(angle));
        float y = static_cast<float>(cy + pixelRadius * std::sin(angle));
        points[i] = {x, y};
    }

    for (int i = 0; i < sides; ++i)
    {
        SDL_RenderLine(context.renderer,
                       points[i].x, points[i].y,
                       points[i + 1].x, points[i + 1].y);
    }
}

void RenderUtils::renderSegment(std::pair<int, int> point1, std::pair<int, int> point2, SDL_Color color, const RenderContext &context)
{
    SDL_SetRenderDrawColor(context.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderLine(context.renderer,
                   point1.first, point1.second,
                   point2.first, point2.second);
}

void RenderUtils::renderPolygon(const std::vector<std::pair<int, int>> &vertices, SDL_Color color, const RenderContext &context)
{
    if (vertices.size() < 2)
        return;

    SDL_SetRenderDrawColor(context.renderer, color.r, color.g, color.b, color.a);

    for (size_t i = 0; i < vertices.size() - 1; i++)
    {
        SDL_RenderLine(context.renderer,
                       vertices[i].first, vertices[i].second,
                       vertices[i + 1].first, vertices[i + 1].second);
    }
    SDL_RenderLine(context.renderer,
                   vertices[0].first, vertices[0].second,
                   vertices[vertices.size() - 1].first, vertices[vertices.size() - 1].second);
}

void RenderUtils::renderCapsule(std::pair<int, int> center1, std::pair<int, int> center2, float radius, SDL_Color color, const RenderContext &context)
{
    std::pair<float, float> perpendicular = {-(center1.second - center2.second), center1.first - center2.first};
    float len = pow(pow(perpendicular.first, 2) + pow(perpendicular.second, 2), 0.5f);
    perpendicular = {perpendicular.first / len, perpendicular.second / len};

    std::pair<int, int> p1 = {center1.first + perpendicular.first * radius, center1.second + perpendicular.second * radius};
    std::pair<int, int> p2 = {center1.first - perpendicular.first * radius, center1.second - perpendicular.second * radius};
    std::pair<int, int> p3 = {center2.first + perpendicular.first * radius, center2.second + perpendicular.second * radius};
    std::pair<int, int> p4 = {center2.first - perpendicular.first * radius, center2.second - perpendicular.second * radius};

    renderSegment(p1, p3, color, context);
    renderSegment(p2, p4, color, context);
    renderCircle(center1, radius, color, context);
    renderCircle(center2, radius, color, context);
}

bool RenderUtils::renderJoint(b2JointId jointId, const RenderContext &context)
{
    b2JointType type = b2Joint_GetType(jointId);
    b2BodyId bodyAId = b2Joint_GetBodyA(jointId);
    b2BodyId bodyBId = b2Joint_GetBodyB(jointId);
    b2Vec2 localPointA = b2Joint_GetLocalAnchorA(jointId);
    b2Vec2 localPointB = b2Joint_GetLocalAnchorB(jointId);
    b2Vec2 worldPointA = b2Body_GetWorldPoint(bodyAId, localPointA);
    b2Vec2 worldPointB = b2Body_GetWorldPoint(bodyBId, localPointB);

    if (type == b2JointType::b2_distanceJoint)
    {
        auto pixelPointA = metersToPixels({worldPointA.x, worldPointA.y}, context);
        auto pixelPointB = metersToPixels({worldPointB.x, worldPointB.y}, context);
        renderSegment(pixelPointA, pixelPointB, {100, 100, 100, 255}, context);
    }
    if (type == b2JointType::b2_mouseJoint)
    {
        auto pixelPointA = metersToPixels({worldPointA.x, worldPointA.y}, context);
        auto pixelPointB = metersToPixels({worldPointB.x, worldPointB.y}, context);
        renderSegment(pixelPointA, pixelPointB, {50, 50, 50, 255}, context);
    }
    else if (type == b2JointType::b2_revoluteJoint)
    {
        auto pixelPointA = metersToPixels({worldPointA.x, worldPointA.y}, context);
        auto pixelPointB = metersToPixels({worldPointB.x, worldPointB.y}, context);
        renderCircle(pixelPointA,1,{255,0,0,255},context);
        renderCircle(pixelPointB,2,{0,255,0,255},context);
    }
    else
    {
        return false;
    }
    return true;
}
