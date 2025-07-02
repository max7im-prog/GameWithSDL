#include "bodyPart.hpp"

#include"physicsComponents.hpp"

BodyPart::BodyPart(entt::registry &registry, b2WorldId worldId):registry(registry), worldId(worldId)
{
}

BodyPart::~BodyPart()
{
    for(auto pair: this->bodies){
        this->registry.remove<PhysicsBody>(pair.first);
    }
    this->bodies = {};

    for(auto pair: this->joints){
        this->registry.remove<PhysicsJoint>(pair.first);
    }
    this->joints = {};
}

bool BodyPart::addJointToMap(std::pair<entt::entity, b2JointId> jointPair)
{
    this->joints[jointPair.first] = jointPair.second;
    return true;
}

bool BodyPart::addBodyToMap(std::pair<entt::entity, b2BodyId> bodyPair)
{
    this->bodies[bodyPair.first] = bodyPair.second;
    return true;
}
