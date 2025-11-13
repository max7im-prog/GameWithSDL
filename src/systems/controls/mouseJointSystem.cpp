#include "mouseJointSystem.hpp"
#include "SDL3/SDL.h"
#include "box2d/box2d.h"
#include "box2d/types.h"
#include "emptyShape.hpp"
#include "eventComponents.hpp"
#include "eventUtils.hpp"
#include "gameSystem.hpp"
#include "jointFactory.hpp"
#include "mouseJoint.hpp"
#include "physicsUtils.hpp"
#include "renderUtils.hpp"
#include "shapeFactory.hpp"
MouseJointSystem::MouseJointSystem() : GameSystem("MouseJointSystem") {}

void MouseJointSystem::update(entt::registry &registry,
                              const std::shared_ptr<World> world,
                              const std::shared_ptr<ShapeFactory> shapeFactory,
                              const std::shared_ptr<JointFactory> jointFactory,
                              const RenderContext &renderContext) {

  b2Vec2 mouseLocation;
  {
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    auto meters =
        RenderUtils::screenPosTob2Vec({mouseX, mouseY}, renderContext);
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
              EmptyShapeConfig config;
              config.defaultConfig();
              config.bodyDef.position = mouseLocation;
              config.bodyDef.type = b2_staticBody;
              mouseShape = shapeFactory->create<EmptyShape>(config).lock();
              if (!mouseShape)
                throw std::runtime_error(
                    "Failed to create empty shape for mouse");
            }
            // Mouse joint
            {
              MouseJointConfig config;
              config.defaultConfig();
              config.jointDef.bodyIdA = mouseShape->getBodyId();
              config.jointDef.bodyIdB = b2Shape_GetBody(shapesAtMouse[0]);
              config.jointDef.target = mouseLocation;
              config.jointDef.hertz = 5.0f;
              config.jointDef.dampingRatio = 0.7f;
              config.jointDef.maxForce = b2Body_GetMass(config.jointDef.bodyIdB) *1000.0f;
              mouseJoint = jointFactory->create<MouseJoint>(config).lock();
              if (!mouseShape)
                throw std::runtime_error("Failed to create joint for mouse");
            }
          }
        }
      } else if (input.state == InputState::HELD_PRESSED) {
        // Move Shape to desired location if button is held pressed
        if (jointExists) {
          if (b2Joint_IsValid(mouseJoint->getJointId()) &&
              b2Body_IsValid(mouseShape->getBodyId())) {
            b2BodyId bodyId = mouseShape->getBodyId();
            b2Transform t = b2Body_GetTransform(bodyId);
            t.p = mouseLocation;

            b2Body_SetTransform(bodyId, t.p, t.q);
            b2MouseJoint_SetTarget(mouseJoint->getJointId(), mouseLocation);

            b2BodyId targetBody = b2Joint_GetBodyB(mouseJoint->getJointId());
            b2Body_SetAwake(targetBody, true);
          }
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