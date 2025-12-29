#include "controllerUpdateSystem.hpp"

#include "SDL3/SDL.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_mouse.h"
#include "box2d/math_functions.h"
#include "controlComponents.hpp"
#include "creature.hpp"
#include "eventComponents.hpp"
#include "renderUtils.hpp"

ControllerUpdateSystem::ControllerUpdateSystem()
    : GameSystem("ControllerUpdateSystem") {}

void ControllerUpdateSystem::update(entt::registry &registry,
                                    const RenderContext &renderContext,
                                    double secondsPassed) {
  if (!shouldRun(secondsPassed)) {
    return;
  }

  // Get presses data
  auto controllerView = registry.view<Controller>();
  auto keyPressView = registry.view<PlayerInput, KeyPressEvent>();
  auto buttonPressView = registry.view<PlayerInput, ButtonPressEvent>();

  using KeyInfo = struct KeyInfo {
    SDL_Keycode _keyCode = 0;
    InputState _inputState = InputState::NOT_PRESSED;
  };
  using ButtonInfo = struct ButtonInfo {
    Uint8 _buttonCode = 0;
    InputState _inputState = InputState::NOT_PRESSED;
  };
  std::vector<KeyInfo> pressedKeys = {};
  for (auto [ent, input, kp] : keyPressView.each()) {
    pressedKeys.push_back(KeyInfo{kp.event.key.key, input.state});
  }
  std::vector<ButtonInfo> pressedButtons = {};
  for (auto [ent, input, bp] : buttonPressView.each()) {
    pressedButtons.push_back({bp.event.button.button, input.state});
  }
  b2Vec2 mouseLocation;
  {
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    auto meters =
        RenderUtils::screenPosTob2Vec({mouseX, mouseY}, renderContext);
    mouseLocation = {meters.x, meters.y};
  }

  // Apply presses data
  for (auto controllerEnt : controllerView) {
    auto &controller = registry.get<Controller>(controllerEnt);

    controller._actions.clear();
    b2Vec2 movementDir = {0, 0};
    b2Vec2 lookWorldPoint = mouseLocation;

    // Iterate keys
    for (auto key : pressedKeys) {
      switch (key._keyCode) {
      case SDLK_W:
        movementDir = b2Add(movementDir, {0, 1});
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

      case SDLK_SPACE:
        controller._actions[CreatureAction::PrimaryMobility] = {};
        controller._actions[CreatureAction::PrimaryMobility]._inputState =
            key._inputState;
        break;

      case SDLK_LSHIFT:
        controller._actions[CreatureAction::SecondaryMobility] = {};
        controller._actions[CreatureAction::SecondaryMobility]._inputState =
            key._inputState;
        break;

      default:
        break;
      }
    }

    // Iterate buttons
    for (auto button : pressedButtons) {
      switch (button._buttonCode) {
      case SDL_BUTTON_LEFT:
        controller._actions[CreatureAction::PrimaryAttack] = {};
        controller._actions[CreatureAction::PrimaryAttack]._inputState =
            button._inputState;
        break;
      case SDL_BUTTON_RIGHT:
        controller._actions[CreatureAction::SecondaryAttack] = {};
        controller._actions[CreatureAction::SecondaryAttack]._inputState =
            button._inputState;
        break;
      default:
        break;
      }
    }

    movementDir = b2Normalize(movementDir);
    controller._moveDir = movementDir;
    controller._lookWorldPoint = lookWorldPoint;
  }
}
