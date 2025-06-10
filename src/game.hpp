#ifndef GAME_HPP
#define GAME_HPP

#include <entt/entt.hpp>

#include "renderContext.hpp"

// Class to combine main logic of a game - registry, systems, rendering.
class Game
{
public:
    Game(int w, int h, int fps);
    ~Game();

    // Used to determine if the game loop is active
    bool isRunning();

    // Initializes Game, including SDL
    bool init();

    // Cleans the resources used by game, i guess
    void clean();
    

    // Calls all event systems
    void handleEvents();

    // Calls all game systems
    void update();

    // Calls all rendering systems
    void render();

    // Waits for time to pass to adjust the framerate
    void deltaTime();

private:
    
    // Window parameters
    int WIDTH = 800;
    int HEIGHT = 600;

    // fps parameters
    int FPS = 60;
    Uint64 frameStart = 0;

    // Used to determine if game loop is active
    bool running = false;

    entt::registry registry;
    RenderContext renderContext;
};



#endif //GAME_HPP