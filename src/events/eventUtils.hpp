#pragma once
#include "entt/entity/fwd.hpp"
#include "eventComponents.hpp"
#include <optional>

class EventUtils {
public:
  // Finds a button press in registry
  static std::optional<entt::entity>
  getButtonPress(entt::registry &registry, Uint8 buttonCode);

  // Finds a key press in registry
  static std::optional<entt::entity> getKeyPress(entt::registry &registry,
                                                  SDL_Keycode keyCode);

private:
  EventUtils() = delete;
  ~EventUtils() = delete;
};