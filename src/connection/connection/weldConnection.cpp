#include "weldConnection.hpp"
#include "weldJoint.hpp"

WeldConnection::WeldConnection(entt::registry &registry,
                               const std::shared_ptr<World> world,
                               const WeldConnectionConfig &config,
                               const std::shared_ptr<ShapeFactory> shapeFactory,
                               const std::shared_ptr<JointFactory> jointFactory)
    : Connection(registry, world) {
  weldJoint = jointFactory->createWeldJoint(config.templateJointCfg);
  registerChild(weldJoint);
}

const std::shared_ptr<WeldJoint> WeldConnection::getWeldJoint() const {
  return weldJoint;
}

WeldConnectionConfig WeldConnectionConfig::defaultConfig() {
  WeldConnectionConfig ret;
  ret.templateJointCfg = WeldJointConfig::defaultConfig();
  return ret;
}
