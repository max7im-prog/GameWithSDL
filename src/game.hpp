#pragma once
#include "bodyFactory.hpp"
#include "creatureControlSystem.hpp"
#include "creatureFactory.hpp"
#include "creatureUpdateSystem.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"
#include "terrainFactory.hpp"


#include <entt/entt.hpp>

// Rendering
#include "renderContext.hpp"
#include "renderBackgroundSystem.hpp"
#include "renderPhysicsBodiesSystem.hpp"
#include "renderPhysicsJointsSystem.hpp"

// Events
#include "pollEventSystem.hpp"
#include "quitSystem.hpp"

// Update
#include "controllerUpdateSystem.hpp"
#include "worldUpdateSystem.hpp"
#include "mouseJointSystem.hpp"



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
    ControllerUpdateSystem controllerUpdateSystem;
    CreatureControlSystem creatureControlSystem;
    MouseJointSystem mouseJointSystem;
    CreatureUpdateSystem creatureUpdateSystem;

    // Render systems
    RenderBackgroundSystem renderBackgroundSystem;
    RenderPhysicsBodiesSystem renderPhysicsBodiesSystem;
    RenderPhysicsJointsSystem renderPhysicsJointsSystem;

    entt::registry registry;
    entt::dispatcher eventDispatcher;
    RenderContext renderContext;

    // Entity factories
    std::shared_ptr<PhysicsFactory> physicsFactory;
    std::shared_ptr<BodyFactory> bodyFactory;
    std::shared_ptr<ConnectionFactory> connectionFactory;
    std::shared_ptr<CreatureFactory> creatureFactory;
    std::shared_ptr<TerrainFactory> terrainFactory;
    std::shared_ptr<JointFactory> jointFactory;
    std::shared_ptr<ShapeFactory> shapeFactory;

    // World
    std::shared_ptr<World> world;
};
