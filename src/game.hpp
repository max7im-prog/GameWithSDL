#pragma once
#include "bodyFactory.hpp"
#include "cleanupSystem.hpp"
#include "creatureControlSystem.hpp"
#include "creatureFactory.hpp"
#include "creatureUpdateSystem.hpp"
#include "jointFactory.hpp"
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
  entt::registry registry;
  // Window parameters
  int WIDTH = 800;
  int HEIGHT = 600;

  // Used to determine if game loop is active
  bool running = false;

  // Event systems
  PollEventSystem pollEventSystem;
  QuitSystem quitSystem;

  // Room manager
  std::shared_ptr<RoomManager> roomManager;

  // Update systems
  WorldUpdateSystem worldUpdateSystem;
  ControllerUpdateSystem controllerUpdateSystem;
  CreatureControlSystem creatureControlSystem;
  MouseJointSystem mouseJointSystem;
  CreatureUpdateSystem creatureUpdateSystem;
  CameraSystem cameraSystem;
  CleanupSystem cleanupSystem;
  RoomLoadSystem roomLoadSystem;
  TexturingSystem _texturingSystem;
  SandboxSystem _sandboxSystem;

  // Render systems
  RenderBackgroundSystem renderBackgroundSystem;
  std::shared_ptr<DebugRenderSystem> debugRenderSystem;
  std::shared_ptr<SceneRenderSystem> _sceneRenderSystem;

  std::shared_ptr<RenderContext> renderContext;

  std::shared_ptr<TextureManager> _textureManager;
  std::shared_ptr<Texturer> _texturer;

  // Entity factories
  std::shared_ptr<BodyFactory> bodyFactory;
  std::shared_ptr<ConnectionFactory> connectionFactory;
  std::shared_ptr<CreatureFactory> creatureFactory;
  std::shared_ptr<TerrainFactory> terrainFactory;
  std::shared_ptr<JointFactory> jointFactory;
  std::shared_ptr<ShapeFactory> shapeFactory;
  std::shared_ptr<WorldFactory> worldFactory;

  // World
  std::shared_ptr<World> world;
};
