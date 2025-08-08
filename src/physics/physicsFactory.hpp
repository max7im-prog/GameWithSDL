#pragma once
#include "capsule.hpp"
#include "circle.hpp"
#include "distanceJoint.hpp"
#include "emptyShape.hpp"
#include "mouseJoint.hpp"
#include "polygon.hpp"
#include "revoluteJoint.hpp"
#include "world.hpp"
#include <entt/entt.hpp>

class PhysicsFactory {
public:
  std::shared_ptr<Circle> createCircle(CircleConfig config);
  std::shared_ptr<Polygon> createPolygon(PolygonConfig config);
  std::shared_ptr<Capsule> createCapsule(CapsuleConfig config);
  std::shared_ptr<EmptyShape> createEmptyShape(const EmptyShapeConfig& config);
  std::shared_ptr<RevoluteJoint>
  createRevoluteJoint(RevoluteJointConfig config);
  std::shared_ptr<DistanceJoint>
  createDistanceJoint(DistanceJointConfig config);
  std::shared_ptr<MouseJoint>
  createMouseJoint(MouseJointConfig config);
  
  PhysicsFactory(entt::registry &registry, const std::shared_ptr<World> world);
  

private:
  PhysicsFactory() = delete;
  PhysicsFactory(PhysicsFactory &other) = delete;
  PhysicsFactory(PhysicsFactory &&other) = delete;
  PhysicsFactory operator=(PhysicsFactory &other) = delete;
  PhysicsFactory operator=(PhysicsFactory &&other) = delete;

  entt::registry &registry;
  const std::shared_ptr<World> world;
};