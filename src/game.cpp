#include "game.hpp"

#include <iostream>

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include "physicsComponents.hpp"
#include "physicsUtils.hpp"
#include "creatureComponents.hpp"
#include"controlComponents.hpp"

#include "humanoid.hpp"

Game::Game(int w, int h, int fps) : WIDTH(w), HEIGHT(h), FPS(fps), running(false)
{
}

Game::~Game()
{
}

bool Game::isRunning()
{
    return this->running;
}

void Game::setRunning(bool val)
{
    this->running = val;
}

bool Game::init()
{
    auto window = SDL_CreateWindow("Game", this->WIDTH, this->HEIGHT, 0);
    if (!window)
    {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        return false;
    }
    auto renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        std::cerr << "Failed to create SDL renderer: " << SDL_GetError() << std::endl;
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

    this->registry.on_destroy<PhysicsBody>().connect<&PhysicsUtils::cleanupPhysicsBody>();
    this->registry.on_destroy<PhysicsWorld>().connect<&PhysicsUtils::cleanupPhysicsWorld>();
    this->registry.on_destroy<PhysicsJoint>().connect<&PhysicsUtils::cleanupPhysicsJoint>();

    // inti systems that require initilalization
    this->mouseJointSystem = MouseJointSystem(this->registry);

    // TODO: remove temp testing code

    // world
    auto wrldDef = b2DefaultWorldDef();
    wrldDef.gravity = (b2Vec2){0.0f, -0.5f};
    auto worldEnt = this->registry.create();
    auto &worldComp = this->registry.emplace_or_replace<PhysicsWorld>(worldEnt);
    worldComp.worldId = b2CreateWorld(&wrldDef);

    // some shapes
    entt::entity ent;

    ent = this->registry.create();
    PhysicsUtils::createPolygonPhysicsBody(registry,
                                           ent,
                                           worldComp.worldId,
                                           (b2Vec2){1.0f, 1.0f},
                                           {{0, 0}, {0, 0.5f}, {14, 0.5f}, {14, 0}},
                                           std::nullopt,
                                           b2_staticBody);
                            
    ent = this->registry.create();
    PhysicsUtils::createCirclePhysicsBody(registry,
                                           ent,
                                           worldComp.worldId,
                                           (b2Vec2){5.0f, 5.0f},
                                           (b2Vec2){0.0f, 0.0f},
                                           0.2f,
                                           std::nullopt,
                                           b2_staticBody);



    // ent = this->registry.create();
    // auto &h1 = this->registry.emplace_or_replace<Creature>(ent);
    // h1.creature = std::unique_ptr<Humanoid>(new Humanoid(this->registry, ent, worldComp.worldId, {2, 4},1,1));
    // h1.creature->aim({5,5},true);


    ent = this->registry.create();
    auto &h2 = this->registry.emplace_or_replace<Creature>(ent);
    h2.creature = std::unique_ptr<Humanoid>(new Humanoid(this->registry, ent, worldComp.worldId, {8, 7},7,5));
    h2.creature->aim({5,5},true);

    // ent = this->registry.create();
    // auto &h3 = this->registry.emplace_or_replace<Creature>(ent);
    // h3.creature = std::unique_ptr<Humanoid>(new Humanoid(this->registry, ent, worldComp.worldId, {8, 4},10,10));
    // h3.creature->aim({5,5},true);
    // this->registry.emplace_or_replace<PlayerControlled>(ent);

    // ent = this->registry.create();
    // auto &h4 = this->registry.emplace_or_replace<Creature>(ent);
    // h4.creature = std::unique_ptr<Humanoid>(new Humanoid(this->registry, ent, worldComp.worldId, {11, 4},6,2));
    // h4.creature->aim({5,5},true);


    return true;
}

void Game::clean()
{
    this->registry.clear();
    SDL_DestroyRenderer(this->renderContext.renderer);
    SDL_DestroyWindow(this->renderContext.window);
}

void Game::handleEvents()
{
    this->pollEventSystem.update(this->registry);
    this->quitSystem.update(this->registry, *this);
}

void Game::update()
{
    // this->physicsBodyCreationSystem.update(this->registry);
    this->controllerUpdateSystem.update(this->registry);
    this->creatureControlSystem.update(this->registry);

    this->creatureUpdateSystem.update(this->registry, this->FPS);
    this->mouseJointSystem.update(this->registry, this->renderContext);

    this->worldUpdateSystem.update(this->registry, this->FPS);
}

void Game::render()
{
    this->renderBackgroundSystem.update(this->registry, this->renderContext);
    this->renderPhysicsBodiesSystem.update(this->registry, this->renderContext);
    this->renderPhysicsJointsSystem.update(this->registry, this->renderContext);

    SDL_RenderPresent(this->renderContext.renderer);
}

void Game::deltaTime()
{
    auto cur = SDL_GetTicks();
    if (cur - frameStart < 1000 / this->FPS)
    {
        SDL_Delay(cur - frameStart);
        frameStart = SDL_GetTicks();
    }
}
