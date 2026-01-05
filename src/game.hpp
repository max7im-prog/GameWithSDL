#pragma once
#include "bodyFactory.hpp"
#include "cleanupSystem.hpp"
#include "creatureControlSystem.hpp"
#include "creatureFactory.hpp"
#include "creatureUpdateSystem.hpp"
#include "jointFactory.hpp"
#include "renderUpdateSystem.hpp"
#include "roomManager.hpp"
#include "sandboxSystem.hpp"
#include "sceneRenderSystem.hpp"
#include "shapeFactory.hpp"
#include "terrainFactory.hpp"

#include <entt/entt.hpp>

// Rendering
#include "debugRenderSystem.hpp"
#include "renderBackgroundSystem.hpp"
#include "renderContext.hpp"
// #include "renderPhysicsJointsSystem.hpp"

// Events
#include "pollEventSystem.hpp"
#include "quitSystem.hpp"

// Update
#include "cameraSystem.hpp"
#include "controllerUpdateSystem.hpp"
#include "mouseJointSystem.hpp"
#include "roomLoadSystem.hpp"
#include "textureManager.hpp"
#include "texturer.hpp"
#include "texturingSystem.hpp"
#include "worldFactory.hpp"
#include "worldUpdateSystem.hpp"

// Class to combine main logic of a game - registry, systems, rendering.
class Game {
public:
  Game(int w, int h);
  ~Game();

  // Used to determine if the game loop is active
  bool isRunning();
  void setRunning(bool val);

  // Initializes Game, including SDL
  bool init();

  /**
   * @brief Initialzed rendering
   */
  bool initRendering();
  /**
   * @brief Initialzed factories, room loaders, texture loaders, etc
   */
  bool initResourceManagers();
  /**
   * @brief Initialzed systems
   */
  bool initSystems();
  /**
   * @brief Initialized the initial structure of the world (rooms, creatures,
   * etc)
   */
  bool initWorld();

  // Cleans all the objects that expire at the end of the tick
  void cleanupTick(Uint64 TPS);

  // Calls all event systems
  void handleEvents(Uint64 TPS);

  // Calls all game systems
  void update(Uint64 TPS);

  // Calls all rendering systems
  void render(Uint64 TPS);

  // Cleans the resources used by game, i guess
  void cleanup();

private:
  entt::registry _registry;

  // Window parameters
  int _WIDTH = 800;
  int _HEIGHT = 600;

  // Used to determine if game loop is active
  bool _running = false;

  // Room manager
  std::shared_ptr<RoomManager> _roomManager;

  // Event systems
  std::shared_ptr<PollEventSystem> _pollEventSystem;
  std::shared_ptr<QuitSystem> _quitSystem;

  // Update systems
  std::shared_ptr<WorldUpdateSystem> _worldUpdateSystem;
  std::shared_ptr<ControllerUpdateSystem> _controllerUpdateSystem;
  std::shared_ptr<CreatureControlSystem> _creatureControlSystem;
  std::shared_ptr<MouseJointSystem> _mouseJointSystem;
  std::shared_ptr<CreatureUpdateSystem> _creatureUpdateSystem;
  std::shared_ptr<CameraSystem> _cameraSystem;
  std::shared_ptr<CleanupSystem> _cleanupSystem;
  std::shared_ptr<RoomLoadSystem> _roomLoadSystem;
  std::shared_ptr<SandboxSystem> _sandboxSystem;

  // Render systems
  std::shared_ptr<RenderBackgroundSystem> _renderBackgroundSystem;
  std::shared_ptr<DebugRenderSystem> _debugRenderSystem;
  std::shared_ptr<SceneRenderSystem> _sceneRenderSystem;
  std::shared_ptr<RenderUpdateSystem> _renderUpdateSystem;
  std::shared_ptr<TexturingSystem> _texturingSystem;

  std::shared_ptr<TextureManager> _textureManager;
  std::shared_ptr<Texturer> _texturer;

  std::shared_ptr<RenderContext> _renderContext;

  // Entity factories
  std::shared_ptr<BodyFactory> _bodyFactory;
  std::shared_ptr<ConnectionFactory> _connectionFactory;
  std::shared_ptr<CreatureFactory> _creatureFactory;
  std::shared_ptr<TerrainFactory> _terrainFactory;
  std::shared_ptr<JointFactory> _jointFactory;
  std::shared_ptr<ShapeFactory> _shapeFactory;
  std::shared_ptr<WorldFactory> _worldFactory;

  // World
  std::shared_ptr<World> _world;
};
