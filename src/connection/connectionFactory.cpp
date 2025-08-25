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
  std::shared_ptr<DistanceConnection> ret = nullptr;
  try {
    ret = std::shared_ptr<DistanceConnection>(
        new DistanceConnection(registry, world, config, physicsFactory));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerConnection(ret);
  return ret;
}

std::shared_ptr<RevoluteConnection> ConnectionFactory::createRevoluteConnection(
    const RevoluteConnectionConfig &config) {
  std::shared_ptr<RevoluteConnection> ret = nullptr;
  try {
    ret = std::shared_ptr<RevoluteConnection>(
        new RevoluteConnection(registry, world, config, physicsFactory));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerConnection(ret);
  return ret;
}

std::shared_ptr<WeldConnection> ConnectionFactory::createWeldConnection(
    const WeldConnectionConfig &config) {
  std::shared_ptr<WeldConnection> ret = nullptr;
  try {
    ret = std::shared_ptr<WeldConnection>(
        new WeldConnection(registry, world, config, physicsFactory));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerConnection(ret);
  return ret;
}

void ConnectionFactory::registerConnection(
    std::shared_ptr<Connection> connection) {
  auto ent = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsConnection>(ent);
  comp.connection = connection;
  connection->setEntity(ent);
}