#include "controllerUpdateSystem.hpp"

#include "SDL3/SDL.h"
#include "box2d/math_functions.h"
#include "controlComponents.hpp"
#include "eventComponents.hpp"
#include "renderUtils.hpp"


void ControllerUpdateSystem::update(entt::registry &registry,
                                    const RenderContext &renderContext) {

  auto controllerView = registry.view<Controller>();
  auto keyPressView = registry.view<PlayerInput, KeyPressEvent>();
  auto buttonPressView = registry.view<PlayerInput, ButtonPressEvent>();

  std::set<SDL_Keycode> pressedKeys = {};
  for (auto [ent, input, kp] : keyPressView.each()) {
    pressedKeys.insert(kp.event.key.key);
  }

  std::set<Uint8> pressedButtons = {};
  for (auto [ent, input, bp] : buttonPressView.each()) {
    pressedButtons.insert(bp.event.button.button);
  }

  b2Vec2 mouseLocation;
  {
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    auto meters =
        RenderUtils::screenPosTob2Vec({mouseX, mouseY}, renderContext);
    mouseLocation = {meters.x, meters.y};
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
      default:
        break;
      }
    }

    newController.aimContext.update = true;
    for (auto button : pressedButtons) {
      switch (button) {
      case SDL_BUTTON_LEFT:
        newController.aimContext.worldPoint = mouseLocation;
        newController.aimContext.aim = true;
        newController.aimContext.update = true;
        break;
      default:
        break;
      }
    }

    {
      newController.lookContext.update = true;
      newController.lookContext.worldPoint = mouseLocation;
      newController.lookContext.look = true;
    }

    movementDir = b2Normalize(movementDir);
    newController.moveContext.moveDir = movementDir;
    newController.moveContext.moveIntensity = 1; // TODO: magic number
    newController.moveContext.update = true;
    controller = newController;
  }
}