#pragma once

#include "connection.hpp"
#include "connectionComponents.hpp"
#include "distanceConnection.hpp"
#include "jointFactory.hpp"
#include "physicsFactory.hpp"
#include "revoluteConnection.hpp"
#include "shapeFactory.hpp"
#include "weldConnection.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
class ConnectionFactory : public RegistryObjectFactory<ConnectionFactory> {
public:
  std::shared_ptr<DistanceConnection>
  createDistanceConnection(const DistanceConnectionConfig &config);
  std::shared_ptr<RevoluteConnection>
  createRevoluteConnection(const RevoluteConnectionConfig &config);
  std::shared_ptr<WeldConnection>
  createWeldConnection(const WeldConnectionConfig &config);
  ConnectionFactory(entt::registry &registry,
                    const std::shared_ptr<World> world,
                    const std::shared_ptr<ShapeFactory> shapeFactory,
                    const std::shared_ptr<JointFactory> jointFactory);

protected:
  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent) {
    auto &comp = registry.emplace_or_replace<PhysicsConnection>(ent);
    comp.connection = object;
  }

  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config) {
    return std::shared_ptr<T>(new T(registry, world, config, shapeFactory, jointFactory));
  }

private:
  const std::shared_ptr<World> world;
  const std::shared_ptr<ShapeFactory> shapeFactory;
  const std::shared_ptr<JointFactory> jointFactory;

  template <typename Derived> friend class RegistryObjectFactory;
};