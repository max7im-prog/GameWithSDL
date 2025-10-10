#pragma once
#include "renderContext.hpp"
#include "renderUtils.hpp"
#include <entt/entt.hpp>

class CameraSystem {
public:
  CameraSystem() = default;
  ~CameraSystem() = default;
  void update(entt::registry &registry, RenderContext &renderContext);

private:
  screenPos initialPressedPos ={0,0};
  b2Vec2 initialBasePos ={0,0};
  bool pressed = false;
  void updatePosition(entt::registry& registry, RenderContext& renderContext);
  void updateZoom(entt::registry& registry, RenderContext& renderContext);
};