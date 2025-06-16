#include <entt/entt.hpp>

#include <box2d/box2d.h>

namespace PhysicsUtils
{
    void cleanupPhysicsBody(entt::registry &registry, entt::entity entity);
    void cleanupPhysicsWorld(entt::registry &registry, entt::entity entity);

    void createPolygonPhysicsShape(entt::registry &registry,
                                   const entt::entity &entity,
                                   b2WorldId worldId,
                                   b2Vec2 position,
                                   const std::vector<b2Vec2> vertices,
                                   b2BodyType bodyType = b2_dynamicBody,
                                   float density = 1.0f,
                                   float friction = 0.3f,
                                   float restitution = 0.3f);
};