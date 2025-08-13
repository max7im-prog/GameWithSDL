#include "controllerUpdateSystem.hpp"

#include <iostream>

#include "SDL3/SDL_keycode.h"
#include "box2d/math_functions.h"
#include "controlComponents.hpp"
#include "eventComponents.hpp"

ControllerUpdateSystem::ControllerUpdateSystem() {}

ControllerUpdateSystem::~ControllerUpdateSystem() {}

void ControllerUpdateSystem::update(entt::registry &registry) {

  auto controllerView = registry.view<Controller>();
  auto keyPressView = registry.view<PlayerInput, KeyPressEvent>();
  auto buttonPressView = registry.view<PlayerInput, ButtonPressEvent>();

  std::set<SDL_Keycode> pressedKeys = {};
  for (auto [ent, input, kp] : keyPressView.each()) {
    pressedKeys.insert(kp.event.key.key);
  }

  for (auto [ent, controller] : controllerView.each()) {
    Controller newController;
    newController.creature = controller.creature;
    b2Vec2 movementDir = {0, 0};
    for (auto key : pressedKeys) {
      switch (key) {
      case SDLK_W:
        movementDir = b2Add(movementDir, {0, 1});
        newController.jump = true;
        break;
      case SDLK_A:
        movementDir = b2Add(movementDir, {-1, 0});
        break;
      case SDLK_S:
        movementDir = b2Add(movementDir, {0, -1});
        break;
      case SDLK_D:
        movementDir = b2Add(movementDir, {1, 0});
        break;
      }
    }
    movementDir = b2Normalize(movementDir);
    newController.moveContext.moveDir = movementDir;
    newController.moveContext.moveIntensity = 1; // TODO: magic number
    newController.moveContext.update = true;
    controller = newController;
  }
}