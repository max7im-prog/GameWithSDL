#include <iostream>

#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include <entt/entt.hpp>

#include "game.hpp"

int main(int argc, char* argv[]) {

    Game game(800,600,60);

    if(!game.init()){
        std::cerr <<"Failed to initialize game" << std::endl;
        return 1;
    }

    while(game.isRunning()){
        game.handleEvents();
        game.update();
        game.render();
        game.deltaTime();
    }

    game.clean();

    return 0;
}
