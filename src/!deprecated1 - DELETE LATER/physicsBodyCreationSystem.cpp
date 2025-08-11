#include "physicsBodyCreationSystem.hpp"

PhysicsBodyCreationSystem::PhysicsBodyCreationSystem()
{
}

PhysicsBodyCreationSystem::~PhysicsBodyCreationSystem()
{
}

void PhysicsBodyCreationSystem::update(entt::registry& registry){
    auto v = registry.view<PendingPhysicsBody>();

    for(auto& ent:v){
        auto& pending = v.get<PendingPhysicsBody>(ent);
        b2BodyId bodyId = b2CreateBody(pending.worldId,&(pending.bodyDef));
        std::vector<b2ShapeId>  shapeIds;
        shapeIds.reserve(pending.shapeDefs.size());
        for(auto& pair:pending.shapeDefs){
            auto shDef = pair.first;
            auto var = pair.second;
            b2ShapeId curShapeId{};
            if(std::holds_alternative<b2Polygon>(var)){
                b2Polygon pol = std::get<b2Polygon>(var);
                curShapeId = b2CreatePolygonShape(bodyId,&shDef,&pol);
            } else if(std::holds_alternative<b2Circle>(var)){
                b2Circle cir = std::get<b2Circle>(var);
                curShapeId = b2CreateCircleShape(bodyId,&shDef,&cir);
            } else if(std::holds_alternative<b2Capsule>(var)){
                b2Capsule caps = std::get<b2Capsule>(var);    
                curShapeId = b2CreateCapsuleShape(bodyId,&shDef,&caps);            
            } else if(std::holds_alternative<b2Segment>(var)){
                b2Segment segm = std::get<b2Segment>(var);
                curShapeId = b2CreateSegmentShape(bodyId,&shDef,&segm);
            }
            shapeIds.push_back(curShapeId);
        }
        auto& comp = registry.emplace_or_replace<PhysicsBody>(ent);
        comp.bodyId = bodyId;
        comp.shapes = shapeIds;
        comp.worldId = pending.worldId;
        registry.remove<PendingPhysicsBody>(ent);
    }

}