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
  ShapeFactory(entt::registry &registry, const std::shared_ptr<World> world);

  template <typename T> static constexpr bool supports() {
    return std::is_same_v<T, Circle> || std::is_same_v<T, Polygon> ||
           std::is_same_v<T, Capsule> || std::is_same_v<T, Segment> ||
           std::is_same_v<T, EmptyShape>;
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

protected:
  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent) {
    auto &comp = registry.emplace_or_replace<PhysicsShape>(ent);
    comp.shape = object;
    world->addObject(std::static_pointer_cast<RegistryObject>(object));
  }

  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config) {
    return std::shared_ptr<T>(new T(registry, *world, config));
  }

private:
  const std::shared_ptr<World> world;

  template <typename> friend class RegistryObjectFactory;
};