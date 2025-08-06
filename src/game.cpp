#include "game.hpp"

#include <iostream>

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include "basicWorld.hpp"
#include "box2d/types.h"
#include "controlComponents.hpp"
// #include "creatureComponents.hpp"
#include "physicsComponents.hpp"
#include "physicsFactory.hpp"
// #include "physicsUtils.hpp"

// #include "humanoid.hpp"
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

  // init systems that require initilalization

  // TODO: remove temp testing code

  // world
  // auto wrldDef = b2DefaultWorldDef();
  // wrldDef.gravity = (b2Vec2){0.0f, -10.0f};
  // auto worldEnt = this->registry.create();
  // auto &worldComp =
  // this->registry.emplace_or_replace<PhysicsWorld>(worldEnt);
  // worldComp.worldId = b2CreateWorld(&wrldDef);

  auto worldEnt = this->registry.create();
  auto &worldComp = this->registry.emplace_or_replace<PhysicsWorld>(worldEnt);
  auto world = std::shared_ptr<World>(new BasicWorld);
  worldComp.world = world;

  // some shapes
  

  auto factory =
      std::shared_ptr<PhysicsFactory>(new PhysicsFactory(registry, world));

  std::shared_ptr<Shape> b0;
  {
    auto config = CircleConfig::defaultConfig();
    config.bodyDef.position = {10, 10};
    config.bodyDef.type = b2_dynamicBody;
    b0 = factory->createCircle(config);
  }
  std::shared_ptr<Shape> b1;
  {
    auto config = PolygonConfig::defaultConfig();
    config.bodyDef.position = {9, 10};
    config.bodyDef.type = b2_dynamicBody;
    b1 = factory->createPolygon(config);
  }
  std::shared_ptr<Shape> b2;
  {
    auto config = CapsuleConfig::defaultConfig();
    config.bodyDef.position = {8, 10};
    config.bodyDef.type = b2_dynamicBody;
    b2 = factory->createCapsule(config);
  }

  {
    auto config = RevoluteJointConfig::defaultConfig();
    config.jointDef.bodyIdA = b1->getBodyId();
    config.jointDef.bodyIdB = b2->getBodyId();
    config.jointDef.localAnchorA = {5, 5};
    config.jointDef.localAnchorB = {5, 6};
    factory->createRevoluteJoint(config);
  }

  {
    auto config = DistanceJointConfig::defaultConfig();
    config.jointDef.bodyIdA = b0->getBodyId();
    config.jointDef.bodyIdB = b2->getBodyId();
    config.jointDef.localAnchorA = {0, 0};
    config.jointDef.localAnchorB = {0, 0};
    config.jointDef.length = 4;
    factory->createDistanceJoint(config);
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
  this->quitSystem.update(this->registry, *this);
}

void Game::update() {
  // this->physicsBodyCreationSystem.update(this->registry);
  this->controllerUpdateSystem.update(this->registry);
  // this->creatureControlSystem.update(this->registry);

  // this->creatureUpdateSystem.update(this->registry, this->FPS);
  // this->mouseJointSystem.update(this->registry, this->renderContext);

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
