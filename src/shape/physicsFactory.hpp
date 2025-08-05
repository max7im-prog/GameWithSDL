#pragma once
#include "circle.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
class PhysicsFactory {
public:
  std::shared_ptr<Circle> createCircle(CircleConfig config);
  PhysicsFactory(entt::registry &registry, const World &world);

private:
  entt::registry &registry;
  const World &world;
};