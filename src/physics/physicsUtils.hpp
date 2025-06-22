#pragma once

#include <entt/entt.hpp>

#include <box2d/box2d.h>

#include <optional>

class PhysicsUtils
{
public:
   static void cleanupPhysicsBody(entt::registry &registry, entt::entity entity);
   static void cleanupPhysicsWorld(entt::registry &registry, entt::entity entity);
   static void cleanupPhysicsJoint(entt::registry &registry, entt::entity entity);

   static void createPolygonPhysicsBody(entt::registry &registry,
                                        const entt::entity &entity,
                                        b2WorldId worldId,
                                        b2Vec2 position,
                                        const std::vector<b2Vec2> vertices,
                                        std::optional<b2Filter> shapeFilter = std::nullopt,
                                        b2BodyType bodyType = b2_dynamicBody,
                                        float density = 1.0f,
                                        float friction = 0.3f,
                                        float restitution = 0.3f);

   static void createCirclePhysicsBody(entt::registry &registry,
                                       const entt::entity &entity,
                                       b2WorldId worldId,
                                       b2Vec2 position,
                                       b2Vec2 center,
                                       float radius,
                                       std::optional<b2Filter> shapeFilter = std::nullopt,
                                       b2BodyType bodyType = b2_dynamicBody,
                                       float density = 1.0f,
                                       float friction = 0.3f,
                                       float restitution = 0.3f);

   static void createCapsulePhysicsBody(entt::registry &registry,
                                        const entt::entity &entity,
                                        b2WorldId worldId,
                                        b2Vec2 position,
                                        b2Vec2 center1,
                                        b2Vec2 center2,
                                        float radius,
                                        std::optional<b2Filter> shapeFilter = std::nullopt,
                                        b2BodyType bodyType = b2_dynamicBody,
                                        float density = 1.0f,
                                        float friction = 0.3f,
                                        float restitution = 0.3f);

   static void createSegmentPhysicsBody(entt::registry &registry,
                                        const entt::entity &entity,
                                        b2WorldId worldId,
                                        b2Vec2 position,
                                        b2Vec2 point1,
                                        b2Vec2 point2,
                                        std::optional<b2Filter> shapeFilter = std::nullopt,
                                        b2BodyType bodyType = b2_dynamicBody,
                                        float density = 1.0f,
                                        float friction = 0.3f,
                                        float restitution = 0.3f);

   static void createDistancePhysicsJoint(entt::registry &registry,
                                          const entt::entity &entity,
                                          b2WorldId worldId,
                                          b2BodyId bodyAId,
                                          b2BodyId bodyBId,
                                          b2Vec2 localPointA,
                                          b2Vec2 localPointB,
                                          std::optional<float> length = std::nullopt,
                                          bool isSpring = false,
                                          float freq = 0.0f,
                                          float damping = 1.0f);
                                    
   static void createPrismaticPhysicsJoint(entt::registry &registry,
                                          const entt::entity &entity,
                                          b2WorldId worldId,
                                          b2BodyId bodyAId,
                                          b2BodyId bodyBId,
                                          b2Vec2 localPointA,
                                          b2Vec2 localPointB,
                                          std::optional<b2Vec2> moveAxisA = std::nullopt,
                                          bool isSpring = false,
                                          float freq = 0.0f,
                                          float damping = 1.0f);

   static void createEmptyPhysicsBody(entt::registry &registry,
                                      const entt::entity &entity,
                                      b2WorldId worldId,
                                      b2Vec2 position,
                                      b2BodyType bodyType = b2_staticBody);

   static void createRevolutePhysicsJoint(entt::registry &registry,
                                          const entt::entity &entity,
                                          b2WorldId worldId,
                                          b2BodyId bodyAId,
                                          b2BodyId bodyBId,
                                          b2Vec2 worldPoint);
   static void createMousePhysicsJoint(entt::registry &registry,
                                       const entt::entity &entity,
                                       b2WorldId worldId,
                                       b2BodyId mouseBodyId,
                                       b2BodyId targetBodyId,
                                       b2Vec2 mouseLocalPoint,
                                       b2Vec2 targetLocalPoint);

   static std::vector<b2ShapeId> getShapeAtPosition(b2WorldId worldId, b2Vec2 position);

   static b2BodyId getBodyId(entt::registry &registry,entt::entity ent);
   static b2JointId getJointId(entt::registry &registry,entt::entity ent);

   // Used to get a unique identifier for a group with disabled collision
   static int getNextNegativeId();

   // Used to get a unique identifier for a group with enabled collision
   static int getNextPositiveId();

   // Apply torgue with a proportional derivative control
   static void applyTorguePD(b2BodyId bodyId, float targetAngle, float kp, float kd);

private:
   // Used in getShapeAtPosition to get a callback from a b2World_OverlapAABB. Context should be a pointer to std::vector<b2ShapeId>
   static bool pointOverlapCallbackFunction(b2ShapeId shapeId, void *context);
};