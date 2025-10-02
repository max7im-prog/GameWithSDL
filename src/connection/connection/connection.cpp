#include "connection.hpp"
#include "registryComposite.hpp"

Connection::Connection(entt::registry &registry,
                       const std::shared_ptr<World> world)
    : RegistryComposite(registry), world(world) {}

Connection::~Connection() {}

void Connection::accept(Visitor &v) { v.visit(this); }