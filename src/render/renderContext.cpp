#include "renderContext.hpp"
#include <stdexcept>

RenderContext::RenderContext(const RenderContextConfig &cfg) {
  window = SDL_CreateWindow(cfg.WindowTitle.c_str(), cfg.widthPixels,
                            cfg.heightPixels, 0);
  if (!window) {
    throw std::runtime_error("Failed to create SDL window");
  }

  renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    SDL_DestroyWindow(window);
    throw std::runtime_error("Failed to create SDL Renderer");
  }

  pixelToMeterRatio = cfg.pixelToMeterRatio;
  basePos = cfg.basePos;
}

RenderContext::~RenderContext() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

RenderContext
RenderContext::createNewRenderContext(const RenderContextConfig &cfg) {
  RenderContext ret(cfg);
  return ret;
}

SDL_Renderer *RenderContext::getSDLRenderer() const{ return renderer; }

SDL_Window *RenderContext::getSDLWindow() const{ return window; }

int RenderContext::getWidthPixels() const{
  int w, h;
  SDL_GetWindowSizeInPixels(window, &w, &h);
  return w;
}

int RenderContext::getHeightPixels() const{
  int w, h;
  SDL_GetWindowSizeInPixels(window, &w, &h);
  return h;
}

float RenderContext::getWidthMeters() const{
  return (float(getWidthPixels())) / pixelToMeterRatio;
}

float RenderContext::getHeightMeters() const{
  return (float(getHeightPixels())) / pixelToMeterRatio;
}

float RenderContext::getPixelToMeterRatio() const{ return pixelToMeterRatio; }

void RenderContext::setPixelToMeterRatio(float ratio) {
  pixelToMeterRatio = ratio;
}

b2Vec2 RenderContext::getBasePos() const{ return basePos; }

void RenderContext::setBasePos(b2Vec2 p) { basePos = p; }