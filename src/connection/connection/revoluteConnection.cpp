#include "revoluteConnection.hpp"
#include "revoluteJoint.hpp"

RevoluteConnection::RevoluteConnection(
    entt::registry &registry, const std::shared_ptr<World> world,
    const RevoluteConnectionConfig &config,
    const std::shared_ptr<ShapeFactory> shapeFactory,
    const std::shared_ptr<JointFactory> jointFactory)
    : Connection(registry, world) {
  revoluteJoint = jointFactory->create<RevoluteJoint>(config.templateJointCfg);
  registerChild(revoluteJoint);
}

const std::shared_ptr<RevoluteJoint>
RevoluteConnection::getRevoluteJoint() const {
  auto ret = revoluteJoint.lock();
  if (!ret)
    throw std::runtime_error("Joint expired");
  return ret;
}

void RevoluteConnectionConfig::defaultConfig() {
  templateJointCfg .defaultConfig();
}

void SegmentConfig::fromJSON(const nlohmann::json& json){
  // TODO: implement
}
