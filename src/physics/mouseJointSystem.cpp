#include "mouseJointSystem.hpp"
#include "eventComponents.hpp"

#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <iostream>

#include "events.hpp"
#include "physicsComponents.hpp"
#include "renderUtils.hpp"
#include "physicsUtils.hpp"
#include "debugUtils.hpp"

MouseJointSystem::MouseJointSystem(entt::registry &registry) : buttonPressed(false), jointExists(false)
{
    this->mouseBody = registry.create();
    registry.emplace_or_replace<Mouse>(this->mouseBody);

    this->mouseJoint = registry.create();
    registry.emplace_or_replace<Mouse>(this->mouseJoint);
}

MouseJointSystem::MouseJointSystem() : buttonPressed(false), jointExists(false)
{
}

MouseJointSystem::~MouseJointSystem()
{
}

void MouseJointSystem::update(entt::registry &registry, const RenderContext &renderContext)
{
    // TODO: rewrite so that it looks at least somewhat decent and not like this mess

    auto presses = registry.view<ButtonPress>();
    bool foundPress = false;
    ButtonPress bp;
    b2Vec2 mouseLocation;
    for (auto ent : presses)
    {
        auto &comp = presses.get<ButtonPress>(ent);
        if (comp.button == SDL_BUTTON_LEFT)
        {
            foundPress = true;
            auto meters = RenderUtils::pixelsToMeters(comp.location, renderContext);
            mouseLocation = {meters.first, meters.second};
            bp = comp;
            break;
        }
    }
    auto worlds = registry.view<PhysicsWorld>();
    bool foundWorld = false;
    b2WorldId worldId;
    for (auto ent : worlds)
    {
        auto &comp = registry.get<PhysicsWorld>(ent);
        worldId = comp.worldId;
        foundWorld = true;
    }

    // If joint exists, move mouse body to the mouse location
    if (buttonPressed && jointExists)
    {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        auto meters = RenderUtils::pixelsToMeters({mouseX, mouseY}, renderContext);
        b2BodyId bodyId = registry.get<PhysicsBody>(this->mouseBody).bodyId;
        b2Transform t = b2Body_GetTransform(bodyId);
        t.p = {meters.first, meters.second};
        b2Body_SetTransform(bodyId, t.p, t.q);

        b2BodyId targetBody = registry.get<PhysicsJoint>(this->mouseJoint).bodyBId;
        b2Body_SetAwake(targetBody,true);
    }

    if (!foundWorld || !foundPress)
    {
        return;
    }

    // Create a new mouse joint on press
    if (!jointExists && !buttonPressed && bp.pressed)
    {
        buttonPressed = true;
        std::vector<b2ShapeId> shapesAtMouse = PhysicsUtils::getShapeAtPosition(worldId, mouseLocation);
        if (shapesAtMouse.size() != 0)
        {

            PhysicsUtils::createEmptyPhysicsBody(registry,
                                                 this->mouseBody,
                                                 worldId,
                                                 mouseLocation);

            b2BodyId bodyA = registry.get<PhysicsBody>(this->mouseBody).bodyId;
            b2BodyId bodyB = b2Shape_GetBody(shapesAtMouse[0]);

            PhysicsUtils::createDistancePhysicsJoint(
                registry,
                this->mouseJoint,
                worldId,
                bodyA,
                bodyB,
                {0, 0},
                b2Body_GetLocalPoint(bodyB, mouseLocation),
                std::nullopt,
                true,
                50,
                1000);
            jointExists = true;

            // std::cout << "created mouse joint and body" << std::endl;
            // std::cout << "body is active: " << b2Body_IsAwake(bodyB)<< std::endl;
            // DebugUtils::debugPrintRegistry(registry);
        }
    }

    // Delete a mouse joint on release
    else
    {
        if (!bp.pressed)
        {

            buttonPressed = false;
            if (jointExists)
            {
                registry.remove<PhysicsJoint>(this->mouseJoint);
                registry.remove<PhysicsBody>(this->mouseBody);

                jointExists = false;
                // std::cout << "removed mouse joint and body" << std::endl;
            }

            // DebugUtils::debugPrintRegistry(registry);
        }
    }
}