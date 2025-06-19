#pragma once
#include <utility>
#include <vector>

#include <box2d/box2d.h>

#include "renderContext.hpp"

#include <SDL3/SDL.h>

namespace RenderUtils
{

    // Function to convert from box2d meters to pixels of a window in RenderContext, returns (x, y)
    std::pair<int, int> metersToPixels(std::pair<float, float> meters, const RenderContext &context);

    // Function to convert from pixels of a window in RenderContext to box2d meters , returns (x, y)
    std::pair<float, float> pixelsToMeters(std::pair<int, int> pixels, const RenderContext &context);

    // Renders a shape, returns true on success, false otherwise
    bool renderShape(b2ShapeId shapeId, const RenderContext &context);

    // Renders a joint, returns true on success, false otherwise
    bool renderJoint(b2JointId jointId, const RenderContext &context);

    void renderCircle(std::pair<int, int> pixelCenter, int pixelRadius, SDL_Color color, const RenderContext &context);
    void renderSegment(std::pair<int, int> pixelPoint1, std::pair<int, int> pixelPoint2, SDL_Color color, const RenderContext &context);
    void renderPolygon(const std::vector<std::pair<int, int>> &pixelVertices, SDL_Color color, const RenderContext &context);
    void renderCapsule(std::pair<int, int> pixelCenter1, std::pair<int, int> pixelCenter2, float pixelRadius, SDL_Color color, const RenderContext &context);

};