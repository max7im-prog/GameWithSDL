#include "weldConnection.hpp"
#include "weldJoint.hpp"

WeldConnection::WeldConnection(entt::registry &registry,
                               const std::shared_ptr<World> world,
                               const WeldConnectionConfig &config,
                               const std::shared_ptr<ShapeFactory> shapeFactory,
                               const std::shared_ptr<JointFactory> jointFactory)
    : Connection(registry, world) {
  weldJoint = jointFactory->create<WeldJoint>(config.templateJointCfg);
  registerChild(weldJoint);
}

const std::shared_ptr<WeldJoint> WeldConnection::getWeldJoint() const {
  auto ret = weldJoint.lock();
  if (!ret)
    throw std::runtime_error("Joint expired");
  return ret;
}

void WeldConnectionConfig::defaultConfig() { templateJointCfg.defaultConfig(); }

void WeldConnectionConfig::fromJSON(const nlohmann::json &json) {
  // TODO: implement
}
