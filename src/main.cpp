#include <iostream>

#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include <entt/entt.hpp>

#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_timer.h"
#include "game.hpp"
#include <SDL3_image/SDL_image.h>
#include <stdexcept>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error("Failed to initialize SDL");
  }

  Game game(1400, 900);

  if (!game.init()) {
    throw std::runtime_error("Failed to initialize game");
  }
  const Uint64 targetTPS = 60;
  const Uint64 targetFPS = 75;

  Uint64 currentTime{SDL_GetPerformanceCounter()};
  Uint64 freq{SDL_GetPerformanceFrequency()};
  Uint64 accumulator{0};
  Uint64 targetTickTime{freq / targetTPS};
  Uint64 targetFrameTime{freq / targetFPS};
  const Uint64 maxFrameTime{freq / 4};
  Uint64 lastRenderTime{currentTime};

  while (game.isRunning()) {

    Uint64 newTime{SDL_GetPerformanceCounter()};
    Uint64 frameTime{std::min(newTime - currentTime, maxFrameTime)};
    currentTime = newTime;
    accumulator += frameTime;

    // Update according to TPS
    while (accumulator >= targetTickTime) {
      game.handleEvents(targetTPS);
      game.update(targetTPS);
      game.cleanupTick(targetTPS);
      accumulator -= targetTickTime;
    }

    // Render according to FPS
    Uint64 sinceLastRender{newTime - lastRenderTime};
    if (sinceLastRender >= targetFrameTime) {
      game.render(targetTPS);
      lastRenderTime = SDL_GetPerformanceCounter();
    } else {
      // Time remaining until next render in seconds
      double remainingSec =
          double(targetFrameTime - sinceLastRender) / double(freq);
      if (remainingSec > 0.002) { // >2ms left? -> sleep
        SDL_Delay(static_cast<Uint32>((remainingSec - 0.001) * 1000.0));
      }
      // Then spin briefly until exact frame time reached
      while (SDL_GetPerformanceCounter() - lastRenderTime < targetFrameTime) {
        SDL_Delay(0); // yield thread (doesn’t busy spin)
      }
    }
  }

  game.cleanup();
  SDL_Quit();

  return 0;
}
