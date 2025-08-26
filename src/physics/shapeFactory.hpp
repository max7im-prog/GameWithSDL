#pragma once
#include "capsule.hpp"
#include "circle.hpp"
#include "emptyShape.hpp"
#include "physicsComponents.hpp"
#include "polygon.hpp"
#include "segment.hpp"
#include "world.hpp"
#include <entt/entt.hpp>

class ShapeFactory : public RegistryObjectFactory<ShapeFactory> {
public:
  std::shared_ptr<Circle> createCircle(const CircleConfig &config);
  std::shared_ptr<Polygon> createPolygon(const PolygonConfig &config);
  std::shared_ptr<Capsule> createCapsule(const CapsuleConfig &config);
  std::shared_ptr<Segment> createSegment(const SegmentConfig &config);
  std::shared_ptr<EmptyShape> createEmptyShape(const EmptyShapeConfig &config);

  ShapeFactory(entt::registry &registry, const std::shared_ptr<World> world);

protected:
  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent) {
    auto &comp = registry.emplace_or_replace<PhysicsShape>(ent);
    comp.shape = object;
  }

  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config) {
    return std::shared_ptr<T>(new T(registry, *world, config));
  }

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
  const std::shared_ptr<World> world;

  template <typename> friend class RegistryObjectFactory;
};