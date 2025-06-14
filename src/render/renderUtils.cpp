#include "renderUtils.hpp"

std::pair<float, float> RenderUtils::metersToPixels(std::pair<float, float> meters, const RenderContext &context)
{
    std::pair<float, float> ret = {0,0};// first - x, second - y
    ret.first = (meters.first - context.x)*context.PixelToMeterRatio;

    float topMeters = (context.y +context.h/context.PixelToMeterRatio);
    ret.second = (topMeters-meters.second) * context.PixelToMeterRatio;

    return ret;
}

std::pair<float, float> RenderUtils::pixelsToMeters(std::pair<float, float> pixels, const RenderContext &context)
{
    std::pair<float, float> ret = {0,0};// first - x, second - y
    ret.first = context.x + pixels.first/context.PixelToMeterRatio;
    ret.second = context.y + context.h/context.PixelToMeterRatio - pixels.second/context.PixelToMeterRatio;
    return ret;
}
