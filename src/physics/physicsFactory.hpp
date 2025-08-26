#pragma once
#include "capsule.hpp"
#include "circle.hpp"
#include "distanceJoint.hpp"
#include "emptyShape.hpp"
#include "mouseJoint.hpp"
#include "polygon.hpp"
#include "registryObjectFactory.hpp"
#include "revoluteJoint.hpp"
#include "segment.hpp"
#include "weldJoint.hpp"
#include "world.hpp"
#include <entt/entt.hpp>

class PhysicsFactory : public RegistryObjectFactory<PhysicsFactory> {
public:
  std::shared_ptr<Circle> createCircle(const CircleConfig &config);
  std::shared_ptr<Polygon> createPolygon(const PolygonConfig &config);
  std::shared_ptr<Capsule> createCapsule(const CapsuleConfig &config);
  std::shared_ptr<Segment> createSegment(const SegmentConfig &config);
  std::shared_ptr<EmptyShape> createEmptyShape(const EmptyShapeConfig &config);

  std::shared_ptr<RevoluteJoint>
  createRevoluteJoint(const RevoluteJointConfig &config);
  std::shared_ptr<DistanceJoint>
  createDistanceJoint(const DistanceJointConfig &config);
  std::shared_ptr<MouseJoint> createMouseJoint(const MouseJointConfig &config);
  std::shared_ptr<WeldJoint> createWeldJoint(const WeldJointConfig &config);

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