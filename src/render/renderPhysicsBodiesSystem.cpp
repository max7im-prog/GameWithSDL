#include "renderPhysicsBodiesSystem.hpp"

#include "physicsObjects.hpp"

RenderPhysicsBodiesSystem::RenderPhysicsBodiesSystem(){

}

RenderPhysicsBodiesSystem::~RenderPhysicsBodiesSystem(){

}

void RenderPhysicsBodiesSystem::update(entt::registry &registry){
    auto v = registry.view<PhysicsBody>();
    for(auto &ent:v){
        auto &comp = v.get<PhysicsBody>(ent);
        for(auto shId: comp.shapes){
            
        }
    }
}