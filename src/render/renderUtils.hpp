#pragma once
#include <utility>

#include "renderContext.hpp"

namespace RenderUtils{

    // Function to convert from box2d meters to pixels of a window in RenderContext, returns (x, y)
    std::pair<float,float> metersToPixels(std::pair<float,float> meters,const RenderContext &context);

    // Function to convert from pixels of a window in RenderContext to box2d meters , returns (x, y)
    std::pair<float,float> pixelsToMeters(std::pair<float,float> pixels,const RenderContext &context);
};