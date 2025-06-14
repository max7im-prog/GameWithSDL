#include "renderUtils.hpp"

std::pair<float, float> RenderUtils::metersToPixels(std::pair<float, float> meters, const RenderContext &context)
{
    std::pair<float, float> ret = {0,0};// first - x, second - y
    ret.first = (meters.first - context.x)*context.pixelToMeterRatio;

    float topMeters = (context.y +context.h/context.pixelToMeterRatio);
    ret.second = (topMeters-meters.second) * context.pixelToMeterRatio;

    return ret;
}

std::pair<float, float> RenderUtils::pixelsToMeters(std::pair<float, float> pixels, const RenderContext &context)
{
    std::pair<float, float> ret = {0,0};// first - x, second - y
    ret.first = context.x + pixels.first/context.pixelToMeterRatio;
    ret.second = context.y + context.h/context.pixelToMeterRatio - pixels.second/context.pixelToMeterRatio;
    return ret;
}

bool RenderUtils::renderShape(b2ShapeId, const RenderContext &context)
{
    // TODO: implement
    return false;
}
