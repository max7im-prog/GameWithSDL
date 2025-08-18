#pragma once
#include <vector>

#include <box2d/box2d.h>

#include "box2d/math_functions.h"
#include "renderContext.hpp"

#include <SDL3/SDL.h>

typedef b2Vec2 screenPos;

namespace RenderUtils
{

    // Function to convert from box2d meters to pixels of a window in RenderContext, returns (x, y)
    screenPos b2VecToScreenPos(b2Vec2 meters, const RenderContext &context);

    // Function to convert from pixels of a window in RenderContext to box2d meters , returns (x, y)
    b2Vec2 screenPosTob2Vec(screenPos pixels, const RenderContext &context);

    // Renders a shape, returns true on success, false otherwise
    bool renderShape(b2ShapeId shapeId, const RenderContext &context);

    // Renders a joint, returns true on success, false otherwise
    bool renderJoint(b2JointId jointId, const RenderContext &context);

    void renderCircle(screenPos pixelCenter, int pixelRadius, SDL_Color color, const RenderContext &context);
    void renderSegment(screenPos pixelPoint1, screenPos pixelPoint2, SDL_Color color, const RenderContext &context);
    void renderPolygon(const std::vector<screenPos> &pixelVertices, SDL_Color color, const RenderContext &context);
    void renderCapsule(screenPos pixelCenter1, screenPos pixelCenter2, float pixelRadius, SDL_Color color, const RenderContext &context);

};