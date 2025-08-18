#pragma once
#include "emptyShape.hpp"
#include "mouseJoint.hpp"
#include "physicsFactory.hpp"
#include "renderContext.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
#include <memory>

class MouseJointSystem {
public:
  MouseJointSystem() = default;
  ~MouseJointSystem() = default;
  void update(entt::registry &registry, const std::shared_ptr<World> world,
              const std::shared_ptr<PhysicsFactory> physicsFactory,
              const RenderContext &renderContext);

private:
  std::shared_ptr<EmptyShape> mouseShape = nullptr;
  std::shared_ptr<MouseJoint> mouseJoint = nullptr;
  bool jointExists = false;
  bool buttonPressed = false;
};