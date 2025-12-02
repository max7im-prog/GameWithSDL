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
#include "renderContext.hpp"
#include "roomComponents.hpp"
#include "roomManager.hpp"
#include "segmentTerrain.hpp"
#include "shapeFactory.hpp"
#include "terrainFactory.hpp"
#include "textureManager.hpp"

Game::Game(int w, int h) : WIDTH(w), HEIGHT(h), running(false) {}

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

  // Texturing
  _textureManager =
      std::shared_ptr<TextureManager>(new TextureManager(*renderContext));
  _texturer = std::shared_ptr<Texturer>(
      new Texturer(registry, *renderContext, _textureManager));

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

  // Load something into world
  {
    auto rooms =
        roomManager->preloadRoomLayout("res/worldLayout/worldLayout.json");

    roomManager->loadRoom("room_001");
    // roomManager->loadRoom("room_016");
  }
  roomLoadSystem.setUpdateInterval(0.25);

  // Controller and loader for a creature
  {
    if (auto creature =
            roomManager->getEntity<DemoCreature>("room_001/creature_001")
                .lock()) {
      {
        auto ent = registry.create();
        auto &controller = registry.emplace_or_replace<Controller>(ent);

        controller.creature = creature->getEntity();
      }
      {
        float interval = 200;
        auto ent = creature->getEntity();
        auto &loadContext =
            registry.emplace_or_replace<CreatureLoadsRoomsTag>(ent);
        loadContext.loadBorder = {interval, interval};
        loadContext.unloadBorder = {interval * 2, interval * 2};
        loadContext.destroyBorder = {interval * 3, interval * 3};
      }
    }
  }
  // Loader for another creature
  // {
  //   if (auto creature =
  //           roomManager->getEntity<DemoCreature>("room_016/creature_001")
  //               .lock()) {
  //     {
  //       float interval = 200;
  //       auto ent = creature->getEntity();
  //       auto &loadContext =
  //           registry.emplace_or_replace<CreatureLoadsRoomsTag>(ent);
  //       loadContext.loadBorder = {interval, interval};
  //       loadContext.unloadBorder = {interval * 2, interval * 2};
  //       loadContext.destroyBorder = {interval * 3, interval * 3};
  //     }
  //   }
  // }


  return true;
}

void Game::cleanup() { this->registry.clear(); }

void Game::handleEvents(Uint64 TPS) {
  double dt = 1.0 / static_cast<double>(TPS);
  this->pollEventSystem.update(this->registry, dt);
  this->quitSystem.update(this->registry, running, dt);
}

void Game::update(Uint64 TPS) {
  double dt = 1.0 / static_cast<double>(TPS);
  this->controllerUpdateSystem.update(this->registry, *renderContext, dt);
  this->creatureControlSystem.update(this->registry, dt);
  this->roomLoadSystem.update(this->registry, this->roomManager, dt);

  creatureUpdateSystem.update(this->registry, dt);
  mouseJointSystem.update(registry, world, shapeFactory, jointFactory,
                          *renderContext, dt);

  cameraSystem.update(this->registry, *renderContext, dt);

  this->worldUpdateSystem.update(this->registry, dt);
}

void Game::cleanupTick(Uint64 TPS) {
  double dt = 1.0 / static_cast<double>(TPS);
  cleanupSystem.update(this->registry, dt);
}

void Game::render(Uint64 TPS) {
  double dt = 1.0 / static_cast<double>(TPS);
  this->_texturingSystem.update(this->registry, this->_texturer, dt);
  this->renderBackgroundSystem.update(this->registry, *renderContext, dt);
  this->debugRenderSystem->update(this->registry, dt);

  this->_sandboxSystem.update(registry, *renderContext, _textureManager, dt);

  SDL_RenderPresent(this->renderContext->getSDLRenderer());
}
