#pragma once
#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include <string>

struct RenderContextConfig {
  static RenderContextConfig defaultConfig();

  float pixelToMeterRatio = 100;
  int widthPixels;
  int heightPixels;
  b2Vec2 basePos;

  std::string WindowTitle;
};

class RenderContext {
public:
  [[nodiscard]] static RenderContext
  createNewRenderContext(const RenderContextConfig &cfg);

  ~RenderContext();

  SDL_Renderer *getSDLRenderer();
  SDL_Window *getSDLWindow();

  int getWidthPixels();
  int getHeightPixels();

  float getWidthMeters();
  float getHeightMeters();

  float getPixelToMeterRatio();
  void setPixelToMeterRatio(float ratio);

  /**
   * @brief Get the position of a top left corner of the screen in box2d world
   */
  b2Vec2 getBasePos();

  /**
   * @brief Set the position of a top left corner of the screen in box2d world
   */
  void setBasePos(b2Vec2 p);

private:
  RenderContext(const RenderContextConfig &cfg);

  /**
   * @brief Conversion from box2d meters to pixels
   */
  float pixelToMeterRatio;

  /**
   * @brief Position of a top left corner of the screen in box2d world
   */
  b2Vec2 basePos;

  SDL_Renderer *renderer;
  SDL_Window *window;

  RenderContext() = delete;
  RenderContext(const RenderContext &) = delete;
  RenderContext &operator=(const RenderContext &) = delete;
  RenderContext(RenderContext &&) = default;
  RenderContext &operator=(RenderContext &&) = default;
};