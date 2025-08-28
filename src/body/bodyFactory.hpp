#pragma once
#include "bodyComponents.hpp"
#include "capsuleBody.hpp"
#include "circleBody.hpp"
#include "jointFactory.hpp"
#include "limbBody.hpp"
#include "polygonBody.hpp"
#include "segmentBody.hpp"
#include "shapeFactory.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
class BodyFactory : public RegistryObjectFactory<BodyFactory> {
public:
  BodyFactory(entt::registry &registry, const std::shared_ptr<World> world,
              const std::shared_ptr<ShapeFactory> shapeFactory,
              const std::shared_ptr<JointFactory> jointFactory);

  template <typename T> static constexpr bool supports() {
    return std::is_same_v<T, CircleBody> || std::is_same_v<T, CapsuleBody> ||
           std::is_same_v<T, PolygonBody> || std::is_same_v<T, LimbBody> ||
           std::is_same_v<T, SegmentBody>;
  }

protected:
  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent) {
    auto &comp = registry.emplace_or_replace<PhysicsBody>(ent);
    comp.body = object;
    world->addObject(std::static_pointer_cast<RegistryObject>(object));
  }

  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config) {
    return std::shared_ptr<T>(
        new T(registry, world, config, shapeFactory, jointFactory));
  }

private:
  const std::shared_ptr<World> world;
  const std::shared_ptr<ShapeFactory> shapeFactory;
  const std::shared_ptr<JointFactory> jointFactory;

  template <typename Derived> friend class RegistryObjectFactory;
};