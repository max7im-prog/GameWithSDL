#pragma once

#include "connection.hpp"
#include "distanceConnection.hpp"
#include "physicsFactory.hpp"
#include "revoluteConnection.hpp"
#include "weldConnection.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
class ConnectionFactory : public RegistryObjectFactory {
public:
  std::shared_ptr<DistanceConnection>
  createDistanceConnection(const DistanceConnectionConfig &config);
  std::shared_ptr<RevoluteConnection>
  createRevoluteConnection(const RevoluteConnectionConfig &config);
  std::shared_ptr<WeldConnection>
  createWeldConnection(const WeldConnectionConfig &config);
  ConnectionFactory(entt::registry &registry,
                    const std::shared_ptr<World> world,
                    const std::shared_ptr<PhysicsFactory> physicsFactory);

private:
  void registerConnection(std::shared_ptr<Connection> Connection);

  const std::shared_ptr<World> world;
  const std::shared_ptr<PhysicsFactory> physicsFactory;
};