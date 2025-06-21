#include "baseCreature.hpp"
#include "physicsComponents.hpp"
#include "physicsUtils.hpp"

BaseCreature::BaseCreature(entt::registry &registry, entt::entity self,b2WorldId worldId, b2Vec2 position):registry(registry), self(self),worldId(worldId),position(position)
{
}

BaseCreature::~BaseCreature()
{
    for(auto ent:this->joints){
        if(this->registry.valid(ent))
            this->registry.destroy(ent);
    }

    for(auto ent:this->bodies){
        if(this->registry.valid(ent))
            this->registry.destroy(ent);
    }
}

void BaseCreature::move(b2Vec2 direction)
{
}

void BaseCreature::jump()
{
}

bool BaseCreature::connectRevolute(entt::entity first, entt::entity second, b2Vec2 worldPoint)
{
    if(!this->registry.all_of<PhysicsBody>(first) ||!this->registry.all_of<PhysicsBody>(second)){
        return false;
    }
    b2BodyId firstId = PhysicsUtils::getBodyId(this->registry,first);
    b2BodyId secondId = PhysicsUtils::getBodyId(this->registry,second);

    entt::entity joint = this->registry.create();
    PhysicsUtils::createRevolutePhysicsJoint(this->registry,joint,this->worldId,firstId,secondId,worldPoint);

    this->joints.push_back(joint);
    return true;
}
