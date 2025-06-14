#pragma once
#include <utility>

#include <box2d/box2d.h>

#include "renderContext.hpp"

namespace RenderUtils{

    // Function to convert from box2d meters to pixels of a window in RenderContext, returns (x, y)
    std::pair<int, int> metersToPixels(std::pair<float,float> meters,const RenderContext &context);

    // Function to convert from pixels of a window in RenderContext to box2d meters , returns (x, y)
    std::pair<float,float> pixelsToMeters(std::pair<int, int> pixels,const RenderContext &context);

    // Renders a shape, returns true on success, false otherwise
    bool renderShape(b2ShapeId shapeId,const RenderContext &context);

    void renderCircle(std::pair<int,int> pixelCenter, int pixelRadius,const RenderContext &context);
};