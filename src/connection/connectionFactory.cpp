// TODO: rewrite
#include "connectionFactory.hpp"
#include "connectionComponents.hpp"
#include "physicsFactory.hpp"
#include "registryObjectFactory.hpp"
ConnectionFactory::ConnectionFactory(
    entt::registry &registry, const std::shared_ptr<World> world,
    const std::shared_ptr<PhysicsFactory> physicsFactory)
    : RegistryObjectFactory(registry), world(world),
      physicsFactory(physicsFactory) {}

std::shared_ptr<DistanceConnection> ConnectionFactory::createDistanceConnection(
    const DistanceConnectionConfig &config) {
  return create<DistanceConnection>(config);
}

std::shared_ptr<RevoluteConnection> ConnectionFactory::createRevoluteConnection(
    const RevoluteConnectionConfig &config) {
  return create<RevoluteConnection>(config);
}

std::shared_ptr<WeldConnection>
ConnectionFactory::createWeldConnection(const WeldConnectionConfig &config) {
  return create<WeldConnection>(config);
}

