#include "game.hpp"


#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include "basicWorld.hpp"
#include "bodyFactory.hpp"
#include "box2d/math_functions.h"
#include "connectionFactory.hpp"
#include "controlComponents.hpp"
#include "creature.hpp"
#include "creature/creature/demoCreature.hpp"
#include "renderContext.hpp"
#include "roomComponents.hpp"
#include "roomManager.hpp"
#include "shapeFactory.hpp"
#include "terrainFactory.hpp"
#include "textureManager.hpp"

Game::Game(int w, int h) : _WIDTH(w), _HEIGHT(h), _running(false) {}

Game::~Game() {}

bool Game::isRunning() { return this->_running; }

void Game::setRunning(bool val) { this->_running = val; }

bool Game::init() {

  // Initialize render context and renderers
  {
    auto cfg = RenderContextConfig::defaultConfig();
    cfg.basePos = {0, 70};
    cfg.widthPixels = 1200;
    cfg.heightPixels = 900;
    cfg.pixelToMeterRatio = 10;
    cfg.WindowTitle = "Game";
    _renderContext = RenderContext::createNewRenderContext(cfg);
  }
  _debugRenderSystem = std::make_unique<DebugRenderSystem>(*_renderContext);
  _sceneRenderSystem = std::make_unique<SceneRenderSystem>(_registry,*_renderContext);
  

  this->_running = true;

  // World and worldFactory
  _worldFactory = std::shared_ptr<WorldFactory>(new WorldFactory(_registry));
  {
    auto config = BasicWorld::Config::defaultConfig();
    config.substepCount = 16;
    _world = _worldFactory->create<BasicWorld>(config).lock();
    if (!_world)
      throw std::runtime_error("Failed to create world");
  }

  // Texturing
  _textureManager =
      std::shared_ptr<TextureManager>(new TextureManager(*_renderContext));
  _texturer = std::shared_ptr<Texturer>(
      new Texturer(_registry, *_renderContext, _textureManager));

  // Initialize factories
  _shapeFactory =
      std::shared_ptr<ShapeFactory>(new ShapeFactory(_registry, _world));

  _jointFactory =
      std::shared_ptr<JointFactory>(new JointFactory(_registry, _world));

  _bodyFactory = std::shared_ptr<BodyFactory>(
      new BodyFactory(_registry, _world, _shapeFactory, _jointFactory));

  _connectionFactory = std::shared_ptr<ConnectionFactory>{
      new ConnectionFactory(_registry, _world, _shapeFactory, _jointFactory)};

  _creatureFactory = std::shared_ptr<CreatureFactory>(
      new CreatureFactory(_registry, _world, _bodyFactory, _connectionFactory));

  _terrainFactory = std::shared_ptr<TerrainFactory>(
      new TerrainFactory(_registry, _world, _bodyFactory, _connectionFactory));

  _roomManager =
      std::make_shared<RoomManager>(_world, _creatureFactory, _terrainFactory);

  // Load something into world
  {
    auto rooms =
        _roomManager->preloadRoomLayout("res/worldLayout/worldLayout.json");

    _roomManager->loadRoom("room_001");
    // roomManager->loadRoom("room_016");
  }
  _roomLoadSystem.setUpdateInterval(0.25);

  // Controller and loader for a creature
  {
    if (auto creature =
            _roomManager->getEntity<DemoCreature>("room_001/creature_001")
                .lock()) {
      {
        auto ent = _registry.create();
        auto &controller = _registry.emplace_or_replace<Controller>(ent);

        controller.creature = creature->getEntity();
      }
      {
        float interval = 200;
        auto ent = creature->getEntity();
        auto &loadContext =
            _registry.emplace_or_replace<CreatureLoadsRoomsTag>(ent);
        loadContext.loadBorder = {interval, interval};
        loadContext.unloadBorder = {interval * 2, interval * 2};
        loadContext.destroyBorder = {interval * 3, interval * 3};
      }
    }
  }
  // Loader for another creature
  // {
  //   if (auto creature =
  //           roomManager->getEntity<DemoCreature>("room_016/creature_001")
  //               .lock()) {
  //     {
  //       float interval = 200;
  //       auto ent = creature->getEntity();
  //       auto &loadContext =
  //           registry.emplace_or_replace<CreatureLoadsRoomsTag>(ent);
  //       loadContext.loadBorder = {interval, interval};
  //       loadContext.unloadBorder = {interval * 2, interval * 2};
  //       loadContext.destroyBorder = {interval * 3, interval * 3};
  //     }
  //   }
  // }


  _debugRenderSystem->setEnabled(true);

  return true;
}

void Game::cleanup() { this->_registry.clear(); }

void Game::handleEvents(Uint64 TPS) {
  double dt = 1.0 / static_cast<double>(TPS);
  this->_pollEventSystem.update(this->_registry, dt);
  this->_quitSystem.update(this->_registry, _running, dt);
}

void Game::update(Uint64 TPS) {
  double dt = 1.0 / static_cast<double>(TPS);
  this->_controllerUpdateSystem.update(this->_registry, *_renderContext, dt);
  this->_creatureControlSystem.update(this->_registry, dt);
  this->_roomLoadSystem.update(this->_registry, this->_roomManager, dt);

  _creatureUpdateSystem.update(this->_registry, dt);
  _mouseJointSystem.update(_registry, _world, _shapeFactory, _jointFactory,
                          *_renderContext, dt);

  _cameraSystem.update(this->_registry, *_renderContext, dt);

  this->_worldUpdateSystem.update(this->_registry, dt);
}

void Game::cleanupTick(Uint64 TPS) {
  double dt = 1.0 / static_cast<double>(TPS);
  _cleanupSystem.update(this->_registry, dt);
}

void Game::render(Uint64 TPS) {
  double dt = 1.0 / static_cast<double>(TPS);
  this->_texturingSystem.update(this->_registry, this->_texturer, dt);
  this->_renderBackgroundSystem.update(this->_registry, *_renderContext, dt);
  this->_sceneRenderSystem->update(this->_registry, dt);
  this->_debugRenderSystem->update(this->_registry, dt);

  this->_sandboxSystem.update(_registry, *_renderContext, _textureManager, dt);

  SDL_RenderPresent(this->_renderContext->getSDLRenderer());
}
