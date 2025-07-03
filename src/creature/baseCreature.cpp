#include "baseCreature.hpp"
#include "physicsComponents.hpp"
#include "physicsUtils.hpp"

BaseCreature::BaseCreature(entt::registry &registry, entt::entity self,b2WorldId worldId, b2Vec2 position, int groupId):registry(registry), self(self),worldId(worldId),position(position),groupId(groupId), weightKg(0)
{
}

BaseCreature::~BaseCreature()
{
    for(auto [ent,id]:this->joints){
        if(this->registry.valid(ent))
            this->registry.destroy(ent);
    }

    for(auto bp:this->bodyParts){
        for(auto pair:bp->getBodies()){
            if(this->registry.valid(pair.first))
                this->registry.destroy(pair.first);
        }
        for(auto pair:bp->getJoints()){
            if(this->registry.valid(pair.first))
                this->registry.destroy(pair.first);
        }
    }
}



void BaseCreature::jump()
{
}

// entt::entity BaseCreature::createBodyEntity()
// {
//     entt::entity ret = this->registry.create();
//     this->bodies.push_back(ret);
//     return ret;
// }

// std::pair<entt::entity,b2JointId> BaseCreature::connectRevolute(entt::entity first, entt::entity second, b2Vec2 worldPoint)
// {
//     b2JointId ret = b2_nullJointId;
//     if(!this->registry.all_of<PhysicsBody>(first) ||!this->registry.all_of<PhysicsBody>(second)){
//         return ret;
//     }
//     b2BodyId firstId = PhysicsUtils::getBodyId(this->registry,first);
//     b2BodyId secondId = PhysicsUtils::getBodyId(this->registry,second);

//     entt::entity joint = this->registry.create();
//     auto pair = PhysicsUtils::createRevolutePhysicsJoint(this->registry,joint,this->worldId,firstId,secondId,worldPoint);
//     this->joints.push_back(pair);

//     return pair.second;
// }

// std::pair<entt::entity,b2JointId> BaseCreature::connectPrismatic(entt::entity first, entt::entity second, b2Vec2 worldPointFirst, b2Vec2 worldPointSecond)
// {
//     b2JointId ret = b2_nullJointId;
//     if(!this->registry.all_of<PhysicsBody>(first) ||!this->registry.all_of<PhysicsBody>(second)){
//         return ret;
//     }
//     b2BodyId firstId = PhysicsUtils::getBodyId(this->registry,first);
//     b2BodyId secondId = PhysicsUtils::getBodyId(this->registry,second);

//     entt::entity joint = this->registry.create();
//     b2Vec2 localPointFirst = b2Body_GetLocalPoint(firstId, worldPointFirst);
//     b2Vec2 localPointSecond = b2Body_GetLocalPoint(secondId, worldPointSecond);
//     auto pair = PhysicsUtils::createPrismaticPhysicsJoint(this->registry,joint,this->worldId,firstId,secondId,localPointFirst,localPointSecond,std::nullopt, true);
//     this->joints.push_back(pair);

//     return pair.second;
// }

std::pair<entt::entity, b2JointId> BaseCreature::connectRevolute(b2BodyId first, b2BodyId second, b2Vec2 worldPoint)
{
    // return std::pair<entt::entity, b2JointId>();
    auto ent = this->registry.create();
    auto pair = PhysicsUtils::createRevolutePhysicsJoint(this->registry,ent,worldId,first,second,worldPoint);

    return pair;
}

std::pair<entt::entity, b2JointId> BaseCreature::connectPrismatic(b2BodyId first, b2BodyId second, b2Vec2 worldPointFirst, b2Vec2 worldPointSecond)
{
    auto ent = this->registry.create();
    auto pair = PhysicsUtils::createPrismaticPhysicsJoint(this->registry,ent,worldId,first,second,worldPointFirst,worldPointSecond);

    return pair;
}

void BaseCreature::updateWeight()
{
    float total = 0;
    for(auto bp:this->bodyParts){
        for(auto pair: bp->getBodies()){
            total+=b2Body_GetMass(pair.second);
        }
    }
    this->weightKg = total;
}

// void BaseCreature::registerBodyPart(CapsuleBodyPart part)
// {

// }

// void BaseCreature::registerBodyPart(CircleBodyPart part)
// {

// }

// void BaseCreature::registerBodyPart(PolygonBodyPart part)
// {

// }

// void BaseCreature::registerBodyPart(LimbBodyPart part)
// {
// }
