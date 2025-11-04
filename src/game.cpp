#include "game.hpp"

#include <iostream>

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include "basicWorld.hpp"
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "capsuleTerrain.hpp"
#include "connectionFactory.hpp"
#include "controlComponents.hpp"
#include "creature.hpp"
#include "creature/creature/demoCreature.hpp"
#include "physicsComponents.hpp"
#include "polygonTerrain.hpp"
#include "roomManager.hpp"
#include "segmentTerrain.hpp"
#include "shapeFactory.hpp"
#include "terrainFactory.hpp"


Game::Game(int w, int h, int fps)
    : WIDTH(w), HEIGHT(h), FPS(fps), running(false) {}

Game::~Game() {}

bool Game::isRunning() { return this->running; }

void Game::setRunning(bool val) { this->running = val; }

bool Game::init() {

  // Initialize render context and renderers
  {
    auto cfg = RenderContextConfig::defaultConfig();
    cfg.basePos = {0, 70};
    cfg.widthPixels = 1200;
    cfg.heightPixels = 900;
    cfg.pixelToMeterRatio = 10;
    cfg.WindowTitle = "Game";
    renderContext = RenderContext::createNewRenderContext(cfg);
  }
  debugRenderSystem = std::make_unique<DebugRenderSystem>(*renderContext);

  this->running = true;

  // World and worldFactory
  worldFactory = std::shared_ptr<WorldFactory>(new WorldFactory(registry));
  {
    auto config = BasicWorld::Config::defaultConfig();
    config.substepCount = 16;
    world = worldFactory->create<BasicWorld>(config).lock();
    if (!world)
      throw std::runtime_error("Failed to create world");
  }

  // Initialize factories
  shapeFactory =
      std::shared_ptr<ShapeFactory>(new ShapeFactory(registry, world));

  jointFactory =
      std::shared_ptr<JointFactory>(new JointFactory(registry, world));

  bodyFactory = std::shared_ptr<BodyFactory>(
      new BodyFactory(registry, world, shapeFactory, jointFactory));

  connectionFactory = std::shared_ptr<ConnectionFactory>{
      new ConnectionFactory(registry, world, shapeFactory, jointFactory)};

  creatureFactory = std::shared_ptr<CreatureFactory>(
      new CreatureFactory(registry, world, bodyFactory, connectionFactory));

  terrainFactory = std::shared_ptr<TerrainFactory>(
      new TerrainFactory(registry, world, bodyFactory, connectionFactory));

  roomManager =
      std::make_shared<RoomManager>(world, creatureFactory, terrainFactory);

  {
    auto temp = roomManager->preloadRoom("/home/max7im/Projects/GameWithSDL/res/room/room_001.json");
    roomManager->loadRoom(*temp);
  }

  return true;
}

void Game::clean() { this->registry.clear(); }

void Game::handleEvents() {
  this->pollEventSystem.update(this->registry);
  this->quitSystem.update(this->registry, running);
}

void Game::update() {
  this->controllerUpdateSystem.update(this->registry, *renderContext);
  this->creatureControlSystem.update(this->registry);

  creatureUpdateSystem.update(this->registry, this->FPS);
  mouseJointSystem.update(registry, world, shapeFactory, jointFactory,
                          *renderContext);

  cameraSystem.update(this->registry, *renderContext);

  this->worldUpdateSystem.update(this->registry, this->FPS);
}

void Game::cleanupFrame() { cleanupSystem.update(this->registry); }

void Game::render() {
  this->renderBackgroundSystem.update(this->registry, *renderContext);
  // this->renderPhysicsBodiesSystem.update(this->registry,
  // this->renderContext);
  // this->renderPhysicsJointsSystem.update(this->registry,
  // this->renderContext);
  this->debugRenderSystem->update(this->registry);

  SDL_RenderPresent(this->renderContext->getSDLRenderer());
}

void Game::deltaTime() {
  Uint64 cur = SDL_GetTicks();
  Uint64 frameDuration = 1000u / static_cast<Uint64>(this->FPS);

  if (cur - frameStart < frameDuration) {
    SDL_Delay(frameDuration - (cur - frameStart));
  }

  frameStart = SDL_GetTicks();
}
