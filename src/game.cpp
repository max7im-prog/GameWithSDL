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

  // TODO: remove temp testing code

  // Some terrain
  {
    PolygonTerrainConfig config;
    config.defaultConfig();
    config.vertices = {{0, 0}, {0, 1}, {60, 1}, {60, 0}};
    config.position = {2, 3};
    config.rotation = b2MakeRot(0);

    terrainFactory->create<PolygonTerrain>(config);
  }
  {
    PolygonTerrainConfig config;
    config.defaultConfig();
    config.vertices = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    config.position = {30, 30};

    terrainFactory->create<PolygonTerrain>(config);
  }
  {
    SegmentTerrainConfig config;
    config.defaultConfig();
    config.point1 = {0, 0};
    config.point2 = {20, 20};
    config.position = {62, 3};

    terrainFactory->create<SegmentTerrain>(config);
  }
  {
    SegmentTerrainConfig config;
    config.defaultConfig();
    config.point1 = {0, 0};
    config.point2 = {20, 0};
    config.position = {82, 23};

    terrainFactory->create<SegmentTerrain>(config);
  }
  {
    SegmentTerrainConfig config;
    config.defaultConfig();
    config.point1 = {0, 0};
    config.point2 = {0, 20};
    config.position = {102, 23};

    terrainFactory->create<SegmentTerrain>(config);
  }
  {
    SegmentTerrainConfig config;
    config.defaultConfig();
    config.point1 = {0, 0};
    config.point2 = {0, 60};
    config.position = {2, 4};
    terrainFactory->create<SegmentTerrain>(config);
  }
  {
    SegmentTerrainConfig config;
    config.defaultConfig();
    config.point1 = {0, 0};
    config.point2 = {100, -21};
    config.position = {2, 64};
    terrainFactory->create<SegmentTerrain>(config);
  }
  {
    PolygonTerrainConfig config;
    config.defaultConfig();
    config.vertices = {{0, 0}, {0, 1}, {30, 1}, {30, 0}};
    config.position = {10, 25};
    terrainFactory->create<PolygonTerrain>(config);
  }
  {
    CapsuleTerrainConfig config;
    config.defaultConfig();
    config.point1 = {-10, 0};
    config.point2 = {10, 0};
    config.radius = 1;
    config.position = {55, 25};
    terrainFactory->create<CapsuleTerrain>(config);
  }
  {
    CircleTerrainConfig config;
    config.defaultConfig();
    config.radius = 1;
    config.position = {82, 23};
    terrainFactory->create<CircleTerrain>(config);
  }
  {
    CircleTerrainConfig config;
    config.defaultConfig();
    config.radius = 1;
    config.position = {85, 23};
    terrainFactory->create<CircleTerrain>(config);
  }
  {
    CircleTerrainConfig config;
    config.defaultConfig();
    config.radius = 1;
    config.position = {89, 23};
    terrainFactory->create<CircleTerrain>(config);
  }
  {
    CircleTerrainConfig config;
    config.defaultConfig();
    config.radius = 1;
    config.position = {90, 23};
    terrainFactory->create<CircleTerrain>(config);
  }

  // Some creatures
  std::shared_ptr<Creature> c0;
  {
    DemoCreatureConfig config;
    config.defaultConfig();
    config.sizeXMeters = 2;
    config.sizeYMeters = 2;
    config.position = {5, 10};
    c0 = creatureFactory->create<DemoCreature>(config).lock();
    if (!world)
      throw std::runtime_error("Failed to create creature");
  }
  c0->aim({30, 30}, true);
  // c0->remove();

  {
    DemoCreatureConfig config;
    config.defaultConfig();
    config.sizeXMeters = 3;
    config.sizeYMeters = 4;
    config.position = {35, 8};
    c0 = creatureFactory->create<DemoCreature>(config).lock();
    if (!world)
      throw std::runtime_error("Failed to create creature");
  }
  // c0->remove();

  // Controller for the creature
  {
    auto ent = registry.create();
    auto &controller = registry.emplace_or_replace<Controller>(ent);
    controller.creature = c0->getEntity();
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
