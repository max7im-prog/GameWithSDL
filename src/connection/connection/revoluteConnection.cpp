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

const std::weak_ptr<RevoluteJoint>
RevoluteConnection::getRevoluteJoint() const {
  return revoluteJoint;
}

RevoluteConnectionConfig RevoluteConnectionConfig::defaultConfig() {
  RevoluteConnectionConfig ret;
  ret.templateJointCfg = RevoluteJointConfig::defaultConfig();
  return ret;
}
