#include "weldConnection.hpp"
#include "weldJoint.hpp"

WeldConnection::WeldConnection(entt::registry &registry,
                               const std::shared_ptr<World> world,
                               const WeldConnection::Config &config,
                               const std::shared_ptr<ShapeFactory> shapeFactory,
                               const std::shared_ptr<JointFactory> jointFactory)
    : Connection(registry, world) {
  weldJoint = jointFactory->create<WeldJoint>(config.templateJointCfg);
  registerJoint(weldJoint, "weldJoint");
}

const std::shared_ptr<WeldJoint> WeldConnection::getWeldJoint() const {
  auto ret = weldJoint.lock();
  if (!ret)
    throw std::runtime_error("Joint expired");
  return ret;
}

void WeldConnection::Config::defaultConfig() {
  templateJointCfg.defaultConfig();
}
