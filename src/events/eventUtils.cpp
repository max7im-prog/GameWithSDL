#include "eventUtils.hpp"
#include "eventComponents.hpp"
#include <entt/entt.hpp>

std::optional<entt::entity> EventUtils::getButtonPress(entt::registry &registry,
                                                       Uint8 buttonCode) {
  auto v = registry.view<PlayerInput, ButtonPressEvent>();
  for (auto &ent : v) {
    auto bp = registry.get<ButtonPressEvent>(ent);
    if (bp.event.button.button == buttonCode) {
      return std::optional<entt::entity>(ent);
    }
  }
  return std::nullopt;
}

std::optional<entt::entity> EventUtils::getKeyPress(entt::registry &registry,
                                                    SDL_Keycode keyCode) {
  auto v = registry.view<PlayerInput, KeyPressEvent>();
  for (auto &ent : v) {
    auto kp = registry.get<KeyPressEvent>(ent);
    if (kp.event.key.key == keyCode) {
      return std::optional<entt::entity>(ent);
    }
  }
  return std::nullopt;
}