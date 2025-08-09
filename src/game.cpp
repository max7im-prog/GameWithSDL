#include "game.hpp"

#include <iostream>

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include "basicWorld.hpp"
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "box2d/types.h"
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
  this->renderContext.pixelToMeterRatio = 50;

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
    config.polygonConfig.vertices = {{0, 0}, {0, 1}, {14, 1}, {14, 0}};
    config.polygonConfig.bodyDef.type = b2_staticBody;
    config.polygonConfig.bodyDef.position = {1, 1};
    bodyFactory->createPolygonBody(config);
  }

  // std::shared_ptr<BodyPart> b0;
  // {
  //   auto config = CircleBodyConfig::defaultConfig();
  //   config.circleConfig.bodyDef.position = {10, 10};
  //   config.circleConfig.bodyDef.type = b2_dynamicBody;
  //   config.circleConfig.radius = 1;
  //   b0 =bodyFactory->createCircleBody(config);
  // }
  // b0->remove();

  // {
  //   auto config = LimbBodyConfig::defaultConfig();
  //   config.templateCapsuleConfig.bodyDef.type = b2_dynamicBody;
  //   config.basePos = {3, 10};
  //   config.rotation = b2MakeRot(-B2_PI / 4);
  //   config.segments = {{.len = 1, .radius = 0.25},
  //                      {.len = 1, .radius = 0.25},
  //                      {.len = 1, .radius = 0.25},
  //                      {.len = 1, .radius = 0.25}};
  //   b0 = bodyFactory->createLimbBody(config);
  // }
  // b0->remove();

  // {
  //   auto config = CapsuleBodyConfig::defaultConfig();
  //   config.capsuleConfig.center1 = {0, 0};
  //   config.capsuleConfig.center2 = {0, -2};
  //   config.capsuleConfig.radius = 1;
  //   config.capsuleConfig.bodyDef.type = b2_dynamicBody;
  //   config.capsuleConfig.bodyDef.position = {8, 10};
  //   config.capsuleConfig.bodyDef.rotation = b2MakeRot(-B2_PI / 4);
  //   b0 = bodyFactory->createCapsuleBody(config);
  // }
  // b0->remove();

  // {
  //   auto config = PolygonBodyConfig::defaultConfig();
  //   config.polygonConfig.radius = 0;
  //   config.polygonConfig.vertices = {{0, 0}, {2, 0}, {1, -2}};
  //   config.polygonConfig.bodyDef.type = b2_dynamicBody;
  //   config.polygonConfig.bodyDef.position = {5, 10};
  //   b0 = bodyFactory->createPolygonBody(config);
  // }
  // b0->remove();
  std::shared_ptr<Creature> c0;
  {
    auto config = DemoCreatureConfig::defaultConfig();
    config.sizeXMeters = 1;
    config.sizeYMeters = 1;
    config.position = {5, 10};
    c0 = creatureFactory->createDemoCreature(config);
  }
  // c0->remove();

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
  // this->physicsBodyCreationSystem.update(this->registry);
  this->controllerUpdateSystem.update(this->registry);
  // this->creatureControlSystem.update(this->registry);

  // this->creatureUpdateSystem.update(this->registry, this->FPS);
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
