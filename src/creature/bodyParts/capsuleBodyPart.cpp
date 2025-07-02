#include "capsuleBodyPart.hpp"
#include "physicsUtils.hpp"

CapsuleBodyPart::CapsuleBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint, b2Vec2 center1, b2Vec2 center2, float radius, std::optional<b2Filter> shapeFilter) : BodyPart(registry, worldId)
{
    auto ent = this->registry.create();
    auto pair = PhysicsUtils::createCapsulePhysicsBody(this->registry, ent, this->worldId, worldPoint, center1, center2, radius, shapeFilter);
    this->addBodyToMap(pair);
}

CapsuleBodyPart::~CapsuleBodyPart()
{
}

std::vector<std::pair<entt::entity, b2BodyId>> CapsuleBodyPart::getBodies()
{
    std::vector<std::pair<entt::entity, b2BodyId>> ret = {};
    for(auto pair:this->bodies){
        ret.push_back(pair);
    }
    return ret;
}

std::vector<std::pair<entt::entity, b2JointId>> CapsuleBodyPart::getJoints()
{
    std::vector<std::pair<entt::entity, b2JointId>> ret = {};
    for(auto pair:this->joints){
        ret.push_back(pair);
    }
    return ret;
}

