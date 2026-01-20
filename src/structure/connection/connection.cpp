#include "connection.hpp"

Connection::Connection(entt::registry &registry,
                       const std::shared_ptr<World> world)
    : Structure(registry), _world(world) {}

Connection::~Connection() = default;
