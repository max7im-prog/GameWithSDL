#include "game.hpp"

#include <iostream>

#include <SDL3/SDL.h>

//TODO: remove temp declarations
#include "physicsObjects.hpp"
#include <box2d/box2d.h>

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

    // TODO: proper initialization
    auto world = this->registry.create();
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, -10.0f};
    b2WorldId worldId = b2CreateWorld(&worldDef);
    this->registry.emplace<PhysicsWorld>(world,worldId);

    for(int i = 0;i< 8;i++){
        auto body = this->registry.create();

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = (b2Vec2){0.0f+float(i), 4.0f};
        b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

        b2Polygon dynamicBox = b2MakeBox(1.0f, 1.0f);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.material.friction = 0.3f;

        b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);

        this->registry.emplace<RigidBody>(body,bodyId, worldId);
    }


    return true;
}

void Game::clean()
{
    // TODO: proper cleanup
    auto v = this->registry.view<PhysicsWorld>();
    for(auto& ent: v){
        auto& comp = v.get<PhysicsWorld>(ent);
        b2DestroyWorld(comp.worldId);
        this->registry.destroy(ent);
    }
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
    this->worldUpdateSystem.update(this->registry, this->FPS);

    // TODO: remove temporary code
    std::cout <<"step" <<std::endl;
    auto v = this->registry.view<RigidBody>();
    for(auto ent:v){
        auto& comp = v.get<RigidBody>(ent);
        
        auto pos = b2Body_GetPosition(comp.bodyId);
        std::cout << pos.x << pos.y <<std::endl;
    }
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
