#pragma once
#include <vector>

#include <box2d/box2d.h>

#include "box2d/math_functions.h"
#include "commonClasses.hpp"
#include "renderContext.hpp"

#include <SDL3/SDL.h>

/**
 * @brief x - offset from the left, y - offset from the top
 */
typedef SDL_FPoint ScreenPos;

namespace RenderUtils {

// Function to convert from box2d meters to pixels of a window in RenderContext,
// returns (x, y)
ScreenPos b2VecToScreenPos(b2Vec2 meters, const RenderContext &context);

// Function to convert from pixels of a window in RenderContext to box2d meters
// , returns (x, y)
b2Vec2 screenPosTob2Vec(ScreenPos pixels, const RenderContext &context);

void renderCircle(ScreenPos pixelCenter, int pixelRadius, SDL_Color color,
                  const RenderContext &context);
void renderSegment(ScreenPos pixelPoint1, ScreenPos pixelPoint2,
                   SDL_Color color, const RenderContext &context);
void renderPolygon(const std::vector<ScreenPos> &pixelVertices, SDL_Color color,
                   const RenderContext &context);
void renderCapsule(ScreenPos pixelCenter1, ScreenPos pixelCenter2,
                   float pixelRadius, SDL_Color color,
                   const RenderContext &context);

void renderTexture(SDL_Texture *texture, const SDL_FRect &textureRegion,
                   float widthMeters, float heightMeters,
                   const Common::Transform &textureTransform,
                   const RenderContext &context);

}; // namespace RenderUtils