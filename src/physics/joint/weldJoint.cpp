#include "weldJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

void WeldJointConfig::defaultConfig() {
  jointDef = b2DefaultWeldJointDef();
}
void WeldJointConfig::fromJSON(const nlohmann::json& json){
  // TODO: implement
}

WeldJoint::WeldJoint(entt::registry &registry, const World &world,
                     const WeldJointConfig &config)
    : Joint(registry) {
  jointId = b2CreateWeldJoint(world.getWorldId(), &(config.jointDef));
}