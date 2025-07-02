#include "circleBodyPart.hpp"
#include "physicsUtils.hpp"

CircleBodyPart::CircleBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint, float radiusM, std::optional<b2Filter> shapeFilter) : BodyPart(registry, worldId)
{
    auto ent = this->registry.create();
    auto pair = PhysicsUtils::createCirclePhysicsBody(this->registry, ent, this->worldId, worldPoint, {0, 0}, radiusM, shapeFilter);
    this->addBodyToMap(pair);
}

CircleBodyPart::~CircleBodyPart()
{
}



std::vector<std::pair<entt::entity, b2BodyId>> CircleBodyPart::getBodies()
{
    std::vector<std::pair<entt::entity, b2BodyId>> ret = {};
    for(auto pair:this->bodies){
        ret.push_back(pair);
    }
    return ret;
}

std::vector<std::pair<entt::entity, b2JointId>> CircleBodyPart::getJoints()
{
    std::vector<std::pair<entt::entity, b2JointId>> ret = {};
    for(auto pair:this->joints){
        ret.push_back(pair);
    }
    return ret;
}
