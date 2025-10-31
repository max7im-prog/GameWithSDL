#include "mouseJoint.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"

void MouseJointConfig::defaultConfig() {
  jointDef = b2DefaultMouseJointDef();
}
void MouseJointConfig::fromJSON(const nlohmann::json& json){
  // TODO: implement
}

MouseJoint::MouseJoint(entt::registry &registry, const World &world,
                             const MouseJointConfig &config)
    : Joint(registry) {
  jointId = b2CreateMouseJoint(world.getWorldId(), &(config.jointDef));
}