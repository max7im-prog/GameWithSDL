#include "cameraSystem.hpp"
#include "eventComponents.hpp"
#include "renderUtils.hpp"
#include <iostream>

CameraSystem::CameraSystem() : GameSystem("CameraSystem") {}

void CameraSystem::update(entt::registry &registry,
                          RenderContext &renderContext, double secondsPassed) {
  if (!shouldRun(secondsPassed)) {
    return;
  }
  updatePosition(registry, renderContext);
  updateZoom(registry, renderContext);
}

void CameraSystem::updatePosition(entt::registry &registry,
                                  RenderContext &renderContext) {
  auto v = registry.view<ButtonPressEvent>();
  bool mmbPressed = false;
  ScreenPos positionPressed = {0, 0};
  for (auto &ent : v) {
    auto &event = registry.get<ButtonPressEvent>(ent);
    if (event.event.button.button == SDL_BUTTON_MIDDLE) {
      mmbPressed = true;
      positionPressed.x = event.event.button.x;
      positionPressed.y = event.event.button.y;
      break;
    }
  }

  if (mmbPressed && pressed) {
    ScreenPos currentPos;
    SDL_GetMouseState(&(currentPos.x), &(currentPos.y));
    auto deltaX = currentPos.x - initialPressedPos.x;
    auto deltaY = currentPos.y - initialPressedPos.y;
    auto ratio = renderContext.getPixelToMeterRatio();
    b2Vec2 deltaVec = {0, 0};
    deltaVec.x = -deltaX / ratio;
    deltaVec.y = deltaY / ratio;
    renderContext.setBasePos(b2Add(initialBasePos, deltaVec));

  } else if (mmbPressed && !pressed) {
    pressed = true;
    initialPressedPos = positionPressed;
    initialBasePos = renderContext.getBasePos();
  } else if (!mmbPressed && pressed) {
    pressed = false;
  } else if (!mmbPressed && !pressed) {
    // Do nothing
  }
}

void CameraSystem::updateZoom(entt::registry &registry,
                              RenderContext &renderContext) {
  constexpr float ZOOM_PER_STEP = 1.1;
  auto v = registry.view<MouseWheelEvent>();
  for (auto &ent : v) {
    // Calculate zoom
    auto &mv = registry.get<MouseWheelEvent>(ent);
    auto zoom = std::pow(ZOOM_PER_STEP, mv.event.wheel.integer_y);

    // Adjust position of renderContext's base
    auto prevRatio = renderContext.getPixelToMeterRatio();
    ScreenPos mousePos;
    mousePos.x = mv.event.wheel.mouse_x;
    mousePos.y = mv.event.wheel.mouse_y;

    b2Vec2 mouseWorldPos =
        RenderUtils::screenPosTob2Vec(mousePos, renderContext);
    b2Vec2 baseWorldPos = renderContext.getBasePos();
    b2Vec2 offset = b2Sub(baseWorldPos, mouseWorldPos);
    b2Vec2 newOffset = b2MulSV(1.0f / zoom, offset);
    b2Vec2 newBasePos = b2Add(newOffset, mouseWorldPos);
    renderContext.setBasePos(newBasePos);

    // Apply zoom
    renderContext.setPixelToMeterRatio(renderContext.getPixelToMeterRatio() *
                                       zoom);
  }
}