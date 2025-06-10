#include "game.hpp"

#include <iostream>

#include <SDL3/SDL.h>

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

bool Game::init()
{
    auto window = SDL_CreateWindow("Game",800,600,0);
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
    return true;
}

void Game::clean()
{
    SDL_DestroyRenderer(this->renderContext.renderer);
    SDL_DestroyWindow(this->renderContext.window);
}


void Game::handleEvents()
{
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_EVENT_QUIT){
            this->running = false;
        }
    }
}

void Game::update()
{
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
