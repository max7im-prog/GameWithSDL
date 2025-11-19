#pragma once
#include "debugRenderer.hpp"
#include "gameSystem.hpp"
#include "texturer.hpp"
#include <entt/entt.hpp>

class TexturingSystem : public GameSystem {
public:
  TexturingSystem();
  ~TexturingSystem() = default;
  void update(entt::registry &registry, std::shared_ptr<Texturer> texturer,
              double secondsPassed);

private:
};