#include "game.hpp"

#include <iostream>

#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include "physicsComponents.hpp"
#include "physicsUtils.hpp"
#include "creatureComponents.hpp"

#include "humanoid.hpp"

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
    this->registry.on_destroy<PhysicsJoint>().connect<&PhysicsUtils::cleanupPhysicsJoint>();

    // inti systems that require initilalization
    this->mouseJointSystem = MouseJointSystem(this->registry);

    // TODO: remove temp testing code

    // world
    auto wrldDef = b2DefaultWorldDef();
    wrldDef.gravity = (b2Vec2){0.0f, -0.5f};
    auto worldEnt = this->registry.create();
    auto& worldComp = this->registry.emplace_or_replace<PhysicsWorld>(worldEnt);
    worldComp.worldId = b2CreateWorld(&wrldDef);

    // some shapes
    entt::entity ent;

    // ent = this->registry.create();
    // PhysicsUtils::createPolygonPhysicsBody(registry,
    //      ent, 
    //      worldComp.worldId, 
    //      (b2Vec2){70.0f, 50.0f},
    //      {{0,0},{0,10}, {10,10}, {10,0}});

    // ent = this->registry.create();
    // PhysicsUtils::createPolygonPhysicsBody(registry,
    //      ent, 
    //      worldComp.worldId, 
    //      (b2Vec2){50.0f, 50.0f},
    //      {{0,0},{0,10}, {10,10}, {13,0}});
    // b2BodyId b4 = this->registry.get<PhysicsBody>(ent).bodyId;

    // ent = this->registry.create();
    // PhysicsUtils::createPolygonPhysicsBody(registry,
    //      ent, 
    //      worldComp.worldId, 
    //      (b2Vec2){30.0f, 50.0f},
    //      {{0,8},{0,10}, {10,19}, {10,0}});
    // b2BodyId b3 = this->registry.get<PhysicsBody>(ent).bodyId;

    // ent = this->registry.create();
    // PhysicsUtils::createPolygonPhysicsBody(registry,
    //      ent, 
    //      worldComp.worldId, 
    //      (b2Vec2){55.0f, 100.0f},
    //      {{0,8},{0,10}, {10,19}, {10,0}});
    // b2BodyId b2 = this->registry.get<PhysicsBody>(ent).bodyId;
    
    ent = this->registry.create();
    PhysicsUtils::createPolygonPhysicsBody(registry,
         ent, 
         worldComp.worldId, 
         (b2Vec2){10.0f, 10.0f},
         {{0,0},{0,10}, {100,10}, {100,0}},
        b2_staticBody); 
    

    ent = this->registry.create();
    PhysicsUtils::createPolygonPhysicsBody(registry,
         ent, 
         worldComp.worldId, 
         (b2Vec2){0.0f, 10.0f},
         {{0,0},{0,100}, {10,100}, {10,0}},
        b2_staticBody); 
    

    ent = this->registry.create();
    PhysicsUtils::createPolygonPhysicsBody(registry,
         ent, 
         worldComp.worldId, 
         (b2Vec2){0.0f, 10.0f},
         {{100,0},{100,100}, {110,100}, {110,0}},
        b2_staticBody); 


    ent = this->registry.create();
    auto& h = this->registry.emplace_or_replace<Creature>(ent);
    Humanoid *human = new Humanoid(this->registry,ent,worldComp.worldId,{50,50},20,20);
    h.creature = std::unique_ptr<Humanoid>(human);

    // ent = this->registry.create();
    // PhysicsUtils::createCirclePhysicsBody(registry,
    //      ent, 
    //      worldComp.worldId, 
    //      (b2Vec2){45.0f, 80.0f},
    //      {0,0},
    //      10); 
    // b2BodyId b1 = this->registry.get<PhysicsBody>(ent).bodyId;
    
    // ent = this->registry.create();
    // PhysicsUtils::createSegmentPhysicsBody(registry,
    //      ent, 
    //      worldComp.worldId, 
    //      (b2Vec2){70.0f, 57.0f},
    //      {20,-10},
    //      {10,10}); 
    
    
    // ent = this->registry.create();
    // PhysicsUtils::createCapsulePhysicsBody(registry,
    //      ent, 
    //      worldComp.worldId, 
    //      (b2Vec2){35.0f, 200.0f},
    //      {-40,-10},
    //      {10,10},
    //     10); 
    

    // some joints 
    // ent = this->registry.create();
    // PhysicsUtils::createDistancePhysicsJoint(registry,
    //     ent,
    //     worldComp.worldId,
    //     b1,
    //     b2,
    //     {0,0},
    //     {10,10},
    //     std::nullopt,
    //     true,
    //     30,
    //     0.5
    // );

    // ent = this->registry.create();
    // PhysicsUtils::createRevolutePhysicsJoint(registry,
    //     ent,worldComp.worldId,
    //     b3,
    //     b4,
    //     b2Body_GetWorldPoint(b3,{20,20})
    // );


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
    this->quitSystem.update(this->registry,*this);
}

void Game::update()
{
    // this->physicsBodyCreationSystem.update(this->registry);
    this->mouseJointSystem.update(this->registry,this->renderContext);
    this->worldUpdateSystem.update(this->registry, this->FPS);
    
}

void Game::render()
{
    this->renderBackgroundSystem.update(this->registry,this->renderContext);
    this->renderPhysicsBodiesSystem.update(this->registry,this->renderContext);
    this->renderPhysicsJointsSystem.update(this->registry,this->renderContext);

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
