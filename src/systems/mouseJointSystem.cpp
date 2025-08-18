#include "mouseJointSystem.hpp"
#include "SDL3/SDL_mouse.h"
#include "box2d/box2d.h"
#include "box2d/types.h"
#include "emptyShape.hpp"
#include "eventComponents.hpp"
#include "eventUtils.hpp"
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
    auto meters = RenderUtils::screenPosTob2Vec({mouseX, mouseY}, renderContext);
    mouseLocation = {meters.x, meters.y};
  }

  auto view = registry.view<ButtonPressEvent, PlayerInput>();
  for (auto [ent, bp, input] : view.each()) {
    if (bp.event.button.button == SDL_BUTTON_LEFT) {
      if (input.state == InputState::JUST_PRESSED) {
        // Create a joint
        if (!jointExists) {
          auto shapesAtMouse = PhysicsUtils::getShapeAtPosition(
              world->getWorldId(), mouseLocation);
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
        }
      } else if (input.state == InputState::HELD_PRESSED) {
        // Move Shape to desired location if button is held pressed
        if (jointExists) {
          b2BodyId bodyId = mouseShape->getBodyId();
          b2Transform t = b2Body_GetTransform(bodyId);
          t.p = mouseLocation;

          b2Body_SetTransform(bodyId, t.p, t.q);
          b2MouseJoint_SetTarget(mouseJoint->getJointId(), mouseLocation);

          b2BodyId targetBody = b2Joint_GetBodyB(mouseJoint->getJointId());
          b2Body_SetAwake(targetBody, true);
        }

      } else if (input.state == InputState::JUST_RELEASED) {
        // Delete joint on button release
        if (jointExists) {
          jointExists = false;
          mouseJoint->remove();
          mouseShape->remove();
          mouseShape = nullptr;
          mouseJoint = nullptr;
        }
      }

      break;
    }
  }
}