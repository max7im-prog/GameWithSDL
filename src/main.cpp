#include <iostream>

#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include <entt/entt.hpp>

#include "game.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  Game game(1400, 900, 60);

  if (!game.init()) {
    std::cerr << "Failed to initialize game" << std::endl;
    return 1;
  }

  while (game.isRunning()) {
    game.handleEvents();
    game.update();
    game.render();
    game.deltaTime();
  }

  game.clean();

  return 0;
}
