#pragma once
#include "distanceJoint.hpp"
#include "mouseJoint.hpp"
#include "physicsComponents.hpp"
#include "revoluteJoint.hpp"
#include "weldJoint.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
class JointFactory : public RegistryObjectFactory<JointFactory> {
public:
  JointFactory(entt::registry &registry, const std::shared_ptr<World> world);

  template <typename T> static constexpr bool supports() {
    return std::is_same_v<T, WeldJoint> || std::is_same_v<T, DistanceJoint> ||
           std::is_same_v<T, MouseJoint> || std::is_same_v<T, RevoluteJoint>;
  }

protected:
  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent) {
    auto &comp = registry.emplace_or_replace<PhysicsJoint>(ent);
    comp.joint = object;
    world->addObject(std::static_pointer_cast<RegistryObject>(object));
  }

  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config) {
    return std::shared_ptr<T>(new T(registry, *world, config));
  }

private:
  const std::shared_ptr<World> world;

  template <typename> friend class RegistryObjectFactory;
};