#include "body.hpp"
#include "registryComposite.hpp"

Body::~Body() {}

Body::Body(entt::registry &registry, const std::shared_ptr<World> world)
    : RegistryComposite(registry), world(world) {}

void Body::accept(Visitor &v) { v.visit(this); }