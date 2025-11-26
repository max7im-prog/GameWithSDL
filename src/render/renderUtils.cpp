#include "renderUtils.hpp"
#include "SDL3/SDL.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/math_functions.h"
#include "renderContext.hpp"

#include <vector>

ScreenPos RenderUtils::b2VecToScreenPos(b2Vec2 meters,
                                        const RenderContext &context) {
  ScreenPos ret;
  b2Vec2 base = context.getBasePos();
  auto ratio = context.getPixelToMeterRatio();

  ret.x = (meters.x - base.x) * ratio;
  ret.y = (base.y - meters.y) * ratio;
  return ret;
}

b2Vec2 RenderUtils::screenPosTob2Vec(ScreenPos pixels,
                                     const RenderContext &context) {
  b2Vec2 ret;
  b2Vec2 base = context.getBasePos();
  auto ratio = context.getPixelToMeterRatio();

  ret.x = base.x + pixels.x / ratio;
  ret.y = base.y - pixels.y / ratio;
  return ret;
}

void RenderUtils::renderCircle(ScreenPos pixelCenter, int pixelRadius,
                               SDL_Color color, const RenderContext &context) {

  auto renderer = context.getSDLRenderer();

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  const int sides = 16;
  const double angleStep = 2.0 * M_PI / sides;

  int cx = pixelCenter.x;
  int cy = pixelCenter.y;

  SDL_FPoint points[sides + 1]; // SDL_FPoint is preferred in SDL3 for subpixel
                                // rendering

  for (int i = 0; i <= sides; ++i) {
    double angle = i * angleStep;
    float x = static_cast<float>(cx + pixelRadius * std::cos(angle));
    float y = static_cast<float>(cy + pixelRadius * std::sin(angle));
    points[i] = {x, y};
  }

  for (int i = 0; i < sides; ++i) {
    SDL_RenderLine(renderer, points[i].x, points[i].y, points[i + 1].x,
                   points[i + 1].y);
  }
}

void RenderUtils::renderSegment(ScreenPos point1, ScreenPos point2,
                                SDL_Color color, const RenderContext &context) {
  auto renderer = context.getSDLRenderer();
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderLine(renderer, point1.x, point1.y, point2.x, point2.y);
}

void RenderUtils::renderPolygon(const std::vector<ScreenPos> &vertices,
                                SDL_Color color, const RenderContext &context) {

  if (vertices.size() < 2)
    return;

  auto renderer = context.getSDLRenderer();

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

  for (size_t i = 0; i < vertices.size() - 1; i++) {
    SDL_RenderLine(renderer, vertices[i].x, vertices[i].y, vertices[i + 1].x,
                   vertices[i + 1].y);
  }
  SDL_RenderLine(renderer, vertices[0].x, vertices[0].y,
                 vertices[vertices.size() - 1].x,
                 vertices[vertices.size() - 1].y);
}

void RenderUtils::renderCapsule(ScreenPos center1, ScreenPos center2,
                                float radius, SDL_Color color,
                                const RenderContext &context) {
  std::pair<float, float> perpendicular = {-(center1.y - center2.y),
                                           center1.x - center2.x};
  float len =
      pow(pow(perpendicular.first, 2) + pow(perpendicular.second, 2), 0.5f);
  perpendicular = {perpendicular.first / len, perpendicular.second / len};

  ScreenPos p1 = {center1.x + perpendicular.first * radius,
                  center1.y + perpendicular.second * radius};
  ScreenPos p2 = {center1.x - perpendicular.first * radius,
                  center1.y - perpendicular.second * radius};
  ScreenPos p3 = {center2.x + perpendicular.first * radius,
                  center2.y + perpendicular.second * radius};
  ScreenPos p4 = {center2.x - perpendicular.first * radius,
                  center2.y - perpendicular.second * radius};

  renderSegment(p1, p3, color, context);
  renderSegment(p2, p4, color, context);
  renderCircle(center1, radius, color, context);
  renderCircle(center2, radius, color, context);
}

void RenderUtils::renderTexture(SDL_Texture *texture,
                                const SDL_FRect &textureRegion,
                                float widthMeters, float heightMeters,
                                const Common::Transform &textureTransform,
                                const RenderContext &context) {
  if (!texture) {
    // TODO: log error??? or throw???
    return;
  }

  auto renderer = context.getSDLRenderer();

  SDL_FlipMode flip{SDL_FlipMode::SDL_FLIP_NONE};
  b2Rot finalRot =
      b2MulRot(textureTransform._relativeRot, textureTransform._rootRot);

  if (textureTransform._flipX && textureTransform._flipY) {
    finalRot = b2MulRot(finalRot, b2MakeRot(B2_PI));
    flip = SDL_FlipMode::SDL_FLIP_NONE;
  } else if (textureTransform._flipX) {
    flip = SDL_FlipMode::SDL_FLIP_HORIZONTAL;
  } else if (textureTransform._flipY) {
    flip = SDL_FlipMode::SDL_FLIP_VERTICAL;
  } else {
    flip = SDL_FlipMode::SDL_FLIP_NONE;
  }

  SDL_FRect destRect{0, 0, 0, 0};
  auto ratio = context.getPixelToMeterRatio();
  b2Vec2 destRectCenterWorldPos =
      b2Add(textureTransform._originPos, textureTransform._relativePos);
  auto destRectCenterScreenPos =
      RenderUtils::b2VecToScreenPos(destRectCenterWorldPos, context);

  destRect.h = heightMeters * ratio * textureTransform._scaleY;
  destRect.w = widthMeters * ratio * textureTransform._scaleX;
  destRect.x = destRectCenterScreenPos.x - destRect.w / 2;
  destRect.y = destRectCenterScreenPos.y - destRect.h / 2;

  SDL_RenderTextureRotated(renderer, texture, &textureRegion, &destRect,
                           b2Rot_GetAngle(finalRot), nullptr, flip);
}