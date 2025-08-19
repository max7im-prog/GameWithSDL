#pragma once
#include "capsule.hpp"
#include "circle.hpp"
#include "distanceJoint.hpp"
#include "emptyShape.hpp"
#include "mouseJoint.hpp"
#include "polygon.hpp"
#include "registryObjectFactory.hpp"
#include "revoluteJoint.hpp"
#include "world.hpp"
#include <entt/entt.hpp>

class PhysicsFactory : public RegistryObjectFactory {
public:
  std::shared_ptr<Circle> createCircle(CircleConfig config);
  std::shared_ptr<Polygon> createPolygon(PolygonConfig config);
  std::shared_ptr<Capsule> createCapsule(CapsuleConfig config);
  std::shared_ptr<EmptyShape> createEmptyShape(const EmptyShapeConfig &config);
  std::shared_ptr<RevoluteJoint>
  createRevoluteJoint(RevoluteJointConfig config);
  std::shared_ptr<DistanceJoint>
  createDistanceJoint(DistanceJointConfig config);
  std::shared_ptr<MouseJoint> createMouseJoint(MouseJointConfig config);

  PhysicsFactory(entt::registry &registry, const std::shared_ptr<World> world);

  /**
   * @brief Used to get a unique identifier for a group with disabled collision
   *
   */
  static int getNextNegativeId();

  /**
   * @brief Used to get a unique identifier for a group with enabled collision
   *
   */
  static int getNextPositiveId();

private:
  void registerJoint(std::shared_ptr<Joint> joint);
  void registerShape(std::shared_ptr<Shape> shape);
  const std::shared_ptr<World> world;
};