#include "mouseJointSystem.hpp"
#include "SDL3/SDL_mouse.h"
#include "box2d/box2d.h"
#include "box2d/types.h"
#include "emptyShape.hpp"
#include "eventComponents.hpp"
#include "mouseJoint.hpp"
#include "physicsUtils.hpp"
#include "renderUtils.hpp"
void MouseJointSystem::update(
    entt::registry &registry, const std::shared_ptr<World> world,
    const std::shared_ptr<PhysicsFactory> physicsFactory,
    const RenderContext &renderContext) {

  b2Vec2 mouseLocation;
  {
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    auto meters = RenderUtils::pixelsToMeters({mouseX, mouseY}, renderContext);
    mouseLocation = {meters.first, meters.second};
  }

  // Move Shape to desired location if button has not been released
  if (buttonPressed && jointExists) {

    b2BodyId bodyId = mouseShape->getBodyId();
    b2Transform t = b2Body_GetTransform(bodyId);
    t.p = mouseLocation;

    b2Body_SetTransform(bodyId, t.p, t.q);
    b2MouseJoint_SetTarget(mouseJoint->getJointId(), mouseLocation);

    b2BodyId targetBody = b2Joint_GetBodyB(mouseJoint->getJointId());
    b2Body_SetAwake(targetBody, true);
  }

  // Find button press
  ButtonPress press;
  bool foundPress = false;
  auto presses = registry.view<ButtonPress>();
  for (auto ent : presses) {
    auto &comp = registry.get<ButtonPress>(ent);
    if (comp.button == SDL_BUTTON_LEFT) {
      foundPress = true;
      press = comp;
      break;
    }
  }
  if (!foundPress) {
    return;
  }
  // Update state of the button
  buttonPressed = press.pressed;

  if (press.pressed && !jointExists) {
    // Create a joint
    auto shapesAtMouse =
        PhysicsUtils::getShapeAtPosition(world->getWorldId(), mouseLocation);
    if (!shapesAtMouse.empty()) {
      jointExists = true;

      // Empty shape
      {
        auto config = EmptyShapeConfig::defaultConfig();
        config.bodyDef.position = mouseLocation;
        config.bodyDef.type = b2_staticBody;
        mouseShape = physicsFactory->createEmptyShape(config);
      }

      // Mouse joint
      {
        auto config = MouseJointConfig::defaultConfig();
        config.jointDef.bodyIdA = mouseShape->getBodyId();
        config.jointDef.bodyIdB = b2Shape_GetBody(shapesAtMouse[0]);
        config.jointDef.target = mouseLocation;
        config.jointDef.hertz = 5.0f;
        config.jointDef.dampingRatio = 0.7f;
        config.jointDef.maxForce = 1000.0f;
        mouseJoint = physicsFactory->createMouseJoint(config);
      }
    }

  } else if (!press.pressed && jointExists) {
    // Delete a joint
    jointExists = false;
    mouseJoint->remove();
    mouseShape->remove();

    mouseShape = nullptr;
    mouseJoint = nullptr;
  }
}