#include "bodyFactory.hpp"
#include "physicsFactory.hpp"
BodyFactory::BodyFactory(entt::registry &registry,
                         const std::shared_ptr<World> world,
                         const std::shared_ptr<PhysicsFactory> physicsFactory)
    : registry(registry), world(world), physicsFactory(physicsFactory) {}