#include "game.hpp"

#include <iostream>

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include "basicWorld.hpp"
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "connectionFactory.hpp"
#include "controlComponents.hpp"
#include "creature.hpp"
#include "creature/creature/demoCreature.hpp"
#include "physicsComponents.hpp"
#include "polygonTerrain.hpp"
#include "shapeFactory.hpp"
#include "terrainFactory.hpp"

Game::Game(int w, int h, int fps)
    : WIDTH(w), HEIGHT(h), FPS(fps), running(false) {}

Game::~Game() {}

bool Game::isRunning() { return this->running; }

void Game::setRunning(bool val) { this->running = val; }

bool Game::init() {
  auto window = SDL_CreateWindow("Game", this->WIDTH, this->HEIGHT, 0);
  if (!window) {
    std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
    return false;
  }
  auto renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    std::cerr << "Failed to create SDL renderer: " << SDL_GetError()
              << std::endl;
    return false;
  }

  this->renderContext.window = window;
  this->renderContext.renderer = renderer;
  this->renderContext.x = 0;
  this->renderContext.y = 0;
  this->renderContext.w = this->WIDTH;
  this->renderContext.h = this->HEIGHT;
  this->renderContext.pixelToMeterRatio = 12.5f;

  this->running = true;

  // World and worldFactory
  worldFactory = std::shared_ptr<WorldFactory>(new WorldFactory(registry));
  {
    auto config = BasicWorld::Config::defaultConfig();
    world = worldFactory->create<BasicWorld>(config);
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
    auto config = PolygonTerrainConfig::defaultConfig();
    config.vertices = {{0, 0}, {0, 1}, {60, 1}, {60, 0}};
    config.position = {2, 3};

    terrainFactory->create<PolygonTerrain>(config);
  }
  {
    auto config = PolygonTerrainConfig::defaultConfig();
    config.vertices = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    config.position = {30, 30};

    terrainFactory->create<PolygonTerrain>(config);
  }
  {
    auto config = SegmentTerrainConfig::defaultConfig();
    config.point1 = {0, 0};
    config.point2 = {20, 20};
    config.position = {62, 3};

    terrainFactory->create<SegmentTerrain>(config);
  }
  {
    auto config = SegmentTerrainConfig::defaultConfig();
    config.point1 = {0, 0};
    config.point2 = {20, 0};
    config.position = {82, 23};

    terrainFactory->create<SegmentTerrain>(config);
  }
  {
    auto config = SegmentTerrainConfig::defaultConfig();
    config.point1 = {0, 0};
    config.point2 = {0, 20};
    config.position = {102, 23};

    terrainFactory->create<SegmentTerrain>(config);
  }
  {
    auto config = SegmentTerrainConfig::defaultConfig();
    config.point1 = {0, 0};
    config.point2 = {0, 60};
    config.position = {2, 4};
    terrainFactory->create<SegmentTerrain>(config);
  }
  {
    auto config = SegmentTerrainConfig::defaultConfig();
    config.point1 = {0, 0};
    config.point2 = {100, -21};
    config.position = {2, 64};
    terrainFactory->create<SegmentTerrain>(config);
  }
  {
    auto config = PolygonTerrainConfig::defaultConfig();
    config.vertices = {{0, 0}, {0, 1}, {30, 1}, {30, 0}};
    config.position = {10, 25};
    terrainFactory->create<PolygonTerrain>(config);
  }
  {
    auto config = CapsuleTerrainConfig::defaultConfig();
    config.point1 = {-10, 0};
    config.point2 = {10, 0};
    config.radius = 1;
    config.position = {55, 25};
    terrainFactory->create<CapsuleTerrain>(config);
  }
  {
    auto config = CircleTerrainConfig::defaultConfig();
    config.radius = 1;
    config.position = {82, 23};
    terrainFactory->create<CircleTerrain>(config);
  }
  {
    auto config = CircleTerrainConfig::defaultConfig();
    config.radius = 1;
    config.position = {85, 23};
    terrainFactory->create<CircleTerrain>(config);
  }
  {
    auto config = CircleTerrainConfig::defaultConfig();
    config.radius = 1;
    config.position = {89, 23};
    terrainFactory->create<CircleTerrain>(config);
  }
  {
    auto config = CircleTerrainConfig::defaultConfig();
    config.radius = 1;
    config.position = {90, 23};
    terrainFactory->create<CircleTerrain>(config);
  }

  // Some creatures
  std::shared_ptr<Creature> c0;
  {
    auto config = DemoCreatureConfig::defaultConfig();
    config.sizeXMeters = 2;
    config.sizeYMeters = 2;
    config.position = {5, 10};
    c0 = creatureFactory->create<DemoCreature>(config);
  }
  c0->aim({30, 30}, true);
  c0->remove();

  {
    auto config = DemoCreatureConfig::defaultConfig();
    config.sizeXMeters = 3;
    config.sizeYMeters = 4;
    config.position = {35, 8};
    c0 = creatureFactory->create<DemoCreature>(config);
  }
  c0->aim({30, 30}, true);
  // c0->remove();

  // Controller for the creature
  {
    auto ent = registry.create();
    auto &controller = registry.emplace_or_replace<Controller>(ent);
    controller.creature = c0->getEntity();
  }

  return true;
}

void Game::clean() {
  this->registry.clear();
  SDL_DestroyRenderer(this->renderContext.renderer);
  SDL_DestroyWindow(this->renderContext.window);
}

void Game::handleEvents() {
  this->pollEventSystem.update(this->registry);
  this->quitSystem.update(this->registry, running);
}

void Game::update() {
  this->controllerUpdateSystem.update(this->registry, renderContext);
  this->creatureControlSystem.update(this->registry);

  creatureUpdateSystem.update(this->registry, this->FPS);
  mouseJointSystem.update(registry, world, shapeFactory, jointFactory,
                          renderContext);

  this->worldUpdateSystem.update(this->registry, this->FPS);
}

void Game::render() {
  this->renderBackgroundSystem.update(this->registry, this->renderContext);
  this->renderPhysicsBodiesSystem.update(this->registry, this->renderContext);
  this->renderPhysicsJointsSystem.update(this->registry, this->renderContext);

  SDL_RenderPresent(this->renderContext.renderer);
}

void Game::deltaTime() {
  Uint64 cur = SDL_GetTicks();
  Uint64 frameDuration = 1000u / static_cast<Uint64>(this->FPS);

  if (cur - frameStart < frameDuration) {
    SDL_Delay(frameDuration - (cur - frameStart));
  }

  frameStart = SDL_GetTicks();
}
