#pragma once
#include "circle.hpp"
#include "polygon.hpp"
#include "capsule.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
class PhysicsFactory {
public:
  std::shared_ptr<Circle> createCircle(CircleConfig config);
  std::shared_ptr<Polygon> createPolygon(PolygonConfig config);
  std::shared_ptr<Capsule> createCapsule(CapsuleConfig config);
  PhysicsFactory(entt::registry &registry, const std::shared_ptr<World> world);

private:
  entt::registry &registry;
  const std::shared_ptr<World> world;
};