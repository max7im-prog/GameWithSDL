#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include <entt/entt.hpp>

#include "renderContext.hpp"
#include "renderBackgroundSystem.hpp"
#include "renderPhysicsBodiesSystem.hpp"


#include "pollEventSystem.hpp"
#include "quitSystem.hpp"
#include "worldUpdateSystem.hpp"
#include "physicsBodyCreationSystem.hpp"

// Class to combine main logic of a game - registry, systems, rendering.
class Game
{
public:
    Game(int w, int h, int fps);
    ~Game();

    // Used to determine if the game loop is active
    bool isRunning();
    void setRunning(bool val);

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

    // Event systems
    PollEventSystem pollEventSystem;
    QuitSystem quitSystem;

    // Update systems
    WorldUpdateSystem worldUpdateSystem;
    PhysicsBodyCreationSystem physicsBodyCreationSystem;

    // Render systems
    RenderBackgroundSystem renderBackgroundSystem ;
    RenderPhysicsBodiesSystem renderPhysicsBodiesSystem;
    
    entt::registry registry;
    entt::dispatcher eventDispatcher;
    RenderContext renderContext;
};



#endif //GAME_HPP