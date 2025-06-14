#include "game.hpp"

#include <iostream>

#include <SDL3/SDL.h>

#include "physicsObjects.hpp"
#include "physicsUtils.hpp"

Game::Game(int w, int h, int fps): WIDTH(w), HEIGHT(h), FPS(fps), running(false)
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
    auto window = SDL_CreateWindow("Game",this->WIDTH,this->HEIGHT,0);
    if(!window){
        std::cerr <<"Failed to create SDL window: " <<SDL_GetError() <<std::endl;
        return false;
    }
    auto renderer = SDL_CreateRenderer(window,nullptr);
    if(!renderer){
        std::cerr <<"Failed to create SDL renderer: " <<SDL_GetError() <<std::endl;
        return false;
    }

    this->renderContext.window = window;
    this->renderContext.renderer = renderer;
    this->renderContext.x = 0;
    this->renderContext.y = 0;
    this->renderContext.w = this->WIDTH;
    this->renderContext.h = this->HEIGHT;
    this->renderContext.pixelToMeterRatio = 5;

    this->running = true;

    this->registry.on_destroy<PhysicsBody>().connect<&PhysicsUtils::cleanupPhysicsBody>();
    this->registry.on_destroy<PhysicsWorld>().connect<&PhysicsUtils::cleanupPhysicsWorld>();

    // TODO: remove temp testing code

    // world
    auto wrldDef = b2DefaultWorldDef();
    wrldDef.gravity = (b2Vec2){0.0f, -0.5f};
    auto worldEnt = this->registry.create();
    auto& worldComp = this->registry.emplace_or_replace<PhysicsWorld>(worldEnt);
    worldComp.worldId = b2CreateWorld(&wrldDef);

    // single circle body and shape
    auto ent = this->registry.create();
    registry.emplace_or_replace<PendingPhysicsBody>(ent);
    auto &comp = registry.get<PendingPhysicsBody>(ent);
    comp.bodyDef = b2DefaultBodyDef();
    comp.bodyDef.type = b2_dynamicBody;
    comp.bodyDef.position = {100,100};


    auto shDef = b2DefaultShapeDef();
    shDef.density =1;
    comp.shapeDefs = {};
    b2Circle circle;
    circle.center = {0,0};
    circle.radius = 3;
    comp.shapeDefs.push_back({shDef, circle});
    comp.texture = std::nullopt;
    comp.worldId = worldComp.worldId;

    return true;
}

void Game::clean()
{
    SDL_DestroyRenderer(this->renderContext.renderer);
    SDL_DestroyWindow(this->renderContext.window);
}


void Game::handleEvents()
{
    this->pollEventSystem.update(this->registry);
    this->quitSystem.update(this->registry,*this);
}

void Game::update()
{
    this->physicsBodyCreationSystem.update(this->registry);
    this->worldUpdateSystem.update(this->registry, this->FPS);
}

void Game::render()
{
    this->renderBackgroundSystem.update(this->registry,this->renderContext);
    this->renderPhysicsBodiesSystem.update(this->registry,this->renderContext);

    SDL_RenderPresent(this->renderContext.renderer);
}

void Game::deltaTime()
{
    auto cur = SDL_GetTicks();
    if(cur - frameStart < 1000/this->FPS){
        SDL_Delay(cur - frameStart);
        frameStart = SDL_GetTicks();
    }
}
