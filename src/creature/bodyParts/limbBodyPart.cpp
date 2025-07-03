#include "limbBodyPart.hpp"

#include "physicsUtils.hpp"

LimbBodyPart::LimbBodyPart(entt::registry &registry, b2WorldId worldId, b2Vec2 worldPoint1, b2Vec2 worldPoint2, std::vector<std::pair<float, float>> portionRadiusPairs, std::optional<b2Filter> shapeFilter):BodyPart(registry,worldId)
{
    float totalPortions = 0;
    for(auto pair:portionRadiusPairs){
        totalPortions +=pair.first;
    }
    for(auto pair:portionRadiusPairs){
        pair.first =pair.first/totalPortions;
    }
    b2Vec2 limbDirection = b2Normalize(b2Sub(worldPoint2,worldPoint1));
    float limbLen = b2Length(b2Sub(worldPoint2,worldPoint1));
    
    // Creating capsules for a limb
    std::vector<std::pair<b2Vec2,b2BodyId>> createdBodies;
    b2Vec2 curPoint = worldPoint1;
    for(auto prp:portionRadiusPairs){
        float portion = prp.first;
        float radius = prp.second;
        auto ent = registry.create();
        b2Vec2 increment = b2MulSV(limbLen*portion,limbDirection);
        auto pair = PhysicsUtils::createCapsulePhysicsBody(registry,ent,worldId,curPoint,{0,0},b2Add({0,0}, increment),radius,shapeFilter);
        addBody(pair);
        createdBodies.push_back({curPoint,pair.second});

        curPoint = b2Add(curPoint,increment);
    }

    for(int i = 1;i< createdBodies.size();i++){
        auto ent = registry.create();
        auto pair = PhysicsUtils::createRevolutePhysicsJoint(registry,ent,worldId,createdBodies[i-1].second,createdBodies[i].second,createdBodies[i].first);
        addJoint(pair);
    }
}

LimbBodyPart::~LimbBodyPart()
{
}

