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

bool BodyPart::addJoint(std::pair<entt::entity, b2JointId> jointPair)
{
    this->joints.push_back(jointPair);
    return true;
}

bool BodyPart::addBody(std::pair<entt::entity, b2BodyId> bodyPair)
{
    this->bodies.push_back(bodyPair);
    return true;
}

const std::vector<std::pair<entt::entity, b2BodyId>> &BodyPart::getBodies()
{
    return this->bodies;
}

const std::vector<std::pair<entt::entity, b2JointId>> &BodyPart::getJoints()
{
    return this->joints;
}
