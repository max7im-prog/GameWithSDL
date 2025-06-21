#include "humanoid.hpp"
#include "physicsUtils.hpp"
#include "baseCreature.hpp"
#include "physicsComponents.hpp"


Humanoid::Humanoid(entt::registry &registry,
                   entt::entity self,
                   b2WorldId worldId,
                   b2Vec2 position,
                   float sizeXMeters,
                   float sizeYMeters) : BaseCreature(registry, self,worldId,position)
{
    float measureX = sizeXMeters / 17.0f; // I got the proportions from a drawing of a ragdoll
    float measureY = sizeYMeters / 14.5f;
    float measure = std::min(measureX,measureY);

    b2Vec2 posNeckBase = b2Add(position,{0,0});
    b2Vec2 posHeadBase = b2Add(posNeckBase, {0,measureY*1.0f});
    b2Vec2 posTorsoBase = b2Add(posNeckBase,{0,-5.0f* measureY});
    b2Vec2 posLeftShoulder = b2Add(posNeckBase,{-2.0f*measureX,0});
    b2Vec2 posRightShoulder = b2Add(posNeckBase,{2.0f*measureX,0});
    b2Vec2 posLeftElbow = b2Add(posLeftShoulder,{-3.0f*measureX,0});
    b2Vec2 posRightElbow = b2Add(posRightShoulder,{3.0f*measureX,0});
    b2Vec2 posLeftPalm = b2Add(posLeftElbow,{-3.0f*measureX,0});
    b2Vec2 posRightPalm = b2Add(posRightElbow,{3.0f*measureX,0});
    b2Vec2 posLeftHip = b2Add(posTorsoBase,{-1.5f,0});
    b2Vec2 posRightHip = b2Add(posTorsoBase,{1.5f,0});
    b2Vec2 posLeftKnee = b2Add(posLeftHip,{0,-3.0f*measureY});
    b2Vec2 posRightKnee = b2Add(posRightHip,{0,-3.0f*measureY});
    b2Vec2 posLeftFoot = b2Add(posLeftKnee,{0,-3.0f*measureY});
    b2Vec2 posRightFoot = b2Add(posRightKnee,{0,-3.0f*measureY});


    // Neck and torso
    // neckLow = this->registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,neckLow,this->worldId,posNeckBase,{0,0},measure*0.75f); 
    neckHigh = this->registry.create();
    PhysicsUtils::createCapsulePhysicsBody(this->registry,neckHigh,worldId,posNeckBase,{0,0},b2Sub(posHeadBase,position),0.5f*measure);
    this->bodies.push_back(neckHigh);

    head = this->registry.create();
    std::vector<b2Vec2> headShape = {{-1.0f*measureX,0},{-1.0f*measureX,2.0f*measureY},{1.0f*measureX,2.0f*measureY},{1.0f*measureX,0}};
    PhysicsUtils::createPolygonPhysicsBody(this->registry,head,worldId,posHeadBase,headShape);
    this->bodies.push_back(head);

    torso = registry.create();
    std::vector<b2Vec2> torsoShape = {{0,0},{-2.0f*measureX, 5.0f* measureY},{2.0f*measureX, 5.0f* measureY}};
    PhysicsUtils::createPolygonPhysicsBody(this->registry,torso,worldId,posTorsoBase,torsoShape);
    this->bodies.push_back(torso);

    // Joint parts
    // shoulderLeft = registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,shoulderLeft,this->worldId,posLeftShoulder,{0,0},measure*1.0f);
    // shoulderRight = registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,shoulderRight,this->worldId,posRightShoulder,{0,0},measure*1.0f);
    // hipLeft = registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,hipLeft,this->worldId,posLeftHip,{0,0},measure*0.5f);
    // hipRight = registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,hipRight,this->worldId,posRightHip,{0,0},measure*0.5f);
    // kneeLeft = registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,kneeLeft,this->worldId,posLeftKnee,{0,0},measure*0.5f);
    // kneeRight = registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,kneeRight,this->worldId,posRightKnee,{0,0},measure*0.5f);
    // footLeft = registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,footLeft,this->worldId,posLeftFoot,{0,0},measure*0.75f);
    // footRight = registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,footRight,this->worldId,posRightFoot,{0,0},measure*0.75f);
    // elbowLeft = registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,elbowLeft,this->worldId,posLeftElbow,{0,0},measure*0.5f);
    // elbowRight = registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,elbowRight,this->worldId,posRightElbow,{0,0},measure*0.5f);
    // palmLeft = registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,palmLeft,this->worldId,posLeftPalm,{0,0},measure*0.75f);
    // palmRight = registry.create();
    // PhysicsUtils::createCirclePhysicsBody(this->registry,palmRight,this->worldId,posRightPalm,{0,0},measure*0.75f);

    // Limb parts
    upperArmLeft = this->registry.create();
    PhysicsUtils::createCapsulePhysicsBody(this->registry,upperArmLeft,worldId,posLeftShoulder,{0,0},{-3.0f*measureX,0},0.5F*measure);
    this->bodies.push_back(upperArmLeft);

    upperArmRight = this->registry.create();
    PhysicsUtils::createCapsulePhysicsBody(this->registry,upperArmRight,worldId,posRightShoulder,{0,0},{3.0f*measureX,0},0.5F*measure);
    this->bodies.push_back(upperArmRight);

    forearmLeft = this->registry.create();
    PhysicsUtils::createCapsulePhysicsBody(this->registry,forearmLeft,worldId,posLeftElbow,{0,0},{-3.0f*measureX,0},0.5F*measure);
    this->bodies.push_back(forearmLeft);

    forearmRight = this->registry.create();
    PhysicsUtils::createCapsulePhysicsBody(this->registry,forearmRight,worldId,posRightElbow,{0,0},{3.0f*measureX,0},0.5F*measure);
    this->bodies.push_back(forearmRight);

    femurLeft = this->registry.create();
    PhysicsUtils::createCapsulePhysicsBody(this->registry,femurLeft,worldId,posLeftHip,{0,0},{0,-3.0f*measureY},0.5F*measure);
    this->bodies.push_back(femurLeft);

    femurRight = this->registry.create();
    PhysicsUtils::createCapsulePhysicsBody(this->registry,femurRight,worldId,posRightHip,{0,0},{0,-3.0f*measureY},0.5F*measure);
    this->bodies.push_back(femurRight);

    calfLeft = this->registry.create();
    PhysicsUtils::createCapsulePhysicsBody(this->registry,calfLeft,worldId,posLeftKnee,{0,0},{0,-3.0f*measureY},0.5F*measure);
    this->bodies.push_back(calfLeft);

    calfRight = this->registry.create();
    PhysicsUtils::createCapsulePhysicsBody(this->registry,calfRight,worldId,posRightKnee,{0,0},{0,-3.0f*measureY},0.5F*measure);
    this->bodies.push_back(calfRight);

    // Connect limbs
    // connectRevolute(neckLow,neckHigh,posNeckBase);
    connectRevolute(neckHigh,head,posHeadBase);
    connectRevolute(neckHigh,torso,posNeckBase);
    connectRevolute(torso,upperArmLeft, posLeftShoulder);
    connectRevolute(torso,upperArmRight, posRightShoulder);
    connectRevolute(torso, femurLeft,posLeftHip);
    connectRevolute(torso, femurRight,posRightHip);
    connectRevolute(femurLeft,calfLeft,posLeftKnee);
    connectRevolute(femurRight,calfRight,posRightKnee);
    connectRevolute(forearmLeft,upperArmLeft, posLeftElbow);
    connectRevolute(forearmRight,upperArmRight, posRightElbow);

    // connectRevolute(torso,shoulderLeft, posLeftShoulder);
    // connectRevolute(torso,shoulderRight, posLeftShoulder);
}

Humanoid::~Humanoid()
{
}

void Humanoid::aim(b2Vec2 point)
{
}

void Humanoid::attack()
{
}

void Humanoid::defend()
{
}


void Humanoid::update()
{
}
