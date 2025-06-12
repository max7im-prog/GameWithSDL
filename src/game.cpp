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
    this->running = true;

    this->registry.on_destroy<PhysicsBody>().connect<&cleanupPhysicsBody>();
    this->registry.on_destroy<PhysicsWorld>().connect<&cleanupPhysicsWorld>();


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
}

void Game::deltaTime()
{
    auto cur = SDL_GetTicks();
    if(cur - frameStart < 1000/this->FPS){
        SDL_Delay(cur - frameStart);
        frameStart = SDL_GetTicks();
    }
}
