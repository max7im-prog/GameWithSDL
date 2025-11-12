#pragma once
#include "emptyShape.hpp"
#include "gameSystem.hpp"
#include "jointFactory.hpp"
#include "mouseJoint.hpp"
#include "renderContext.hpp"
#include "shapeFactory.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
#include <memory>

class MouseJointSystem :public GameSystem{
public:
  MouseJointSystem() ;
  ~MouseJointSystem() = default;
  void update(entt::registry &registry, const std::shared_ptr<World> world,
              const std::shared_ptr<ShapeFactory> shapeFactory,
              const std::shared_ptr<JointFactory> jointFactory,
              const RenderContext &renderContext);

private:
  std::shared_ptr<EmptyShape> mouseShape = nullptr;
  std::shared_ptr<MouseJoint> mouseJoint = nullptr;
  bool jointExists = false;
  bool buttonPressed = false;
};