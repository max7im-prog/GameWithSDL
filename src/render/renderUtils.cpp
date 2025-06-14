#include "renderUtils.hpp"

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

bool RenderUtils::renderShape(b2ShapeId shapeId,const  RenderContext &context)
{

    auto type = b2Shape_GetType(shapeId);
    auto bodyId = b2Shape_GetBody(shapeId);

    if (type == b2_circleShape)
    {
        auto circle = b2Shape_GetCircle(shapeId);
        auto transform = b2Body_GetTransform(bodyId);
        b2Vec2 worldCenter = b2TransformPoint(transform, circle.center);

        std::pair<int, int> pixelCenter = metersToPixels({worldCenter.x, worldCenter.y}, context);
        int pixelRadius = circle.radius * context.pixelToMeterRatio;

        renderCircle(pixelCenter, pixelRadius, context);
    }
    else if (type == b2_capsuleShape)
    {
        // TODO: implement
    }
    else if (type == b2_segmentShape)
    {
        // TODO: implement
    }
    else if (type == b2_polygonShape)
    {
        // TODO: implement
    }
    else
    {

        return false;
    }

    return true;
}

void RenderUtils::renderCircle(std::pair<int, int> pixelCenter, int pixelRadius,const  RenderContext &context)
{
    SDL_SetRenderDrawColor(context.renderer,0,0,0,255);
    const int sides = 8;
    const double angleStep = 2.0 * M_PI / sides;

    int cx = pixelCenter.first;
    int cy = pixelCenter.second;

    SDL_FPoint points[sides + 1]; // SDL_FPoint is preferred in SDL3 for subpixel rendering

    for (int i = 0; i <= sides; ++i)
    {
        double angle = i * angleStep;
        float x = static_cast<float>(cx + pixelRadius * std::cos(angle));
        float y = static_cast<float>(cy + pixelRadius * std::sin(angle));
        points[i] = { x, y };
    }

    for (int i = 0; i < sides; ++i)
    {
        SDL_RenderLine(context.renderer,
                       points[i].x, points[i].y,
                       points[i + 1].x, points[i + 1].y);
    }
}
