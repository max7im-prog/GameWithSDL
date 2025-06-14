#include <entt/entt.hpp>

namespace PhysicsUtils
{
    void cleanupPhysicsBody(entt::registry &registry, entt::entity entity);
    void cleanupPhysicsWorld(entt::registry &registry, entt::entity entity);
};