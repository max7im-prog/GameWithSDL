#include "connection.hpp"
#include "registryObject.hpp"

Connection::Connection(entt::registry &registry,
                       const std::shared_ptr<World> world)
    : RegistryObject(registry), world(world) {}

Connection::~Connection() {}