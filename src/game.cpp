#include "game.hpp"

#include <iostream>

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include "basicWorld.hpp"
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include "controlComponents.hpp"
#include "creature.hpp"
#include "creature/creature/demoCreature.hpp"
#include "physicsComponents.hpp"
#include "physicsFactory.hpp"
#include "revoluteJoint.hpp"

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

  // TODO: remove temp testing code

  // world
  auto worldEnt = this->registry.create();
  auto &worldComp = this->registry.emplace_or_replace<PhysicsWorld>(worldEnt);
  world = std::shared_ptr<World>(new BasicWorld);
  worldComp.world = world;

  // Initialize factories
  physicsFactory =
      std::shared_ptr<PhysicsFactory>(new PhysicsFactory(registry, world));

  bodyFactory = std::shared_ptr<BodyFactory>(
      new BodyFactory(registry, world, physicsFactory));

  creatureFactory = std::shared_ptr<CreatureFactory>(
      new CreatureFactory(registry, world, physicsFactory, bodyFactory));

  // Some shapes
  {
    auto config = PolygonBodyConfig::defaultConfig();
    config.polygonConfig.radius = 0;
    config.polygonConfig.vertices = {{0, 0}, {0, 1}, {60, 1}, {60, 0}};
    config.polygonConfig.bodyDef.type = b2_staticBody;
    config.polygonConfig.bodyDef.position = {1, 1};
    bodyFactory->createPolygonBody(config);
  }

  {
    auto config = CircleBodyConfig::defaultConfig();
    config.circleConfig.radius = 0.25f;
    config.circleConfig.bodyDef.position = {30, 30};
    config.circleConfig.bodyDef.type = b2_staticBody;
    bodyFactory->createCircleBody(config);
  }

  std::shared_ptr<Creature> c0;
  {
    auto config = DemoCreatureConfig::defaultConfig();
    config.sizeXMeters = 1;
    config.sizeYMeters = 1;
    config.position = {5, 10};
    c0 = creatureFactory->createDemoCreature(config);
  }
  c0->aim({30, 30}, true);
  
  // c0->remove();

  

  {
    auto config = DemoCreatureConfig::defaultConfig();
    config.sizeXMeters = 2;
    config.sizeYMeters = 3;
    config.position = {35, 6};
    c0 = creatureFactory->createDemoCreature(config);
  }
  c0->aim({30, 30}, true);
  // c0->remove();

  // Controller for the creature
  {
    auto ent = registry.create();
    auto& controller = registry.emplace_or_replace<Controller>(ent);
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
  this->controllerUpdateSystem.update(this->registry);
  this->creatureControlSystem.update(this->registry);

  creatureUpdateSystem.update(this->registry, this->FPS);
  mouseJointSystem.update(registry, world, physicsFactory, renderContext);

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
