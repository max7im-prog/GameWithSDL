#include "revoluteConnection.hpp"
#include "revoluteJoint.hpp"

RevoluteConnection::RevoluteConnection(
    entt::registry &registry, const std::shared_ptr<World> world,
    const RevoluteConnectionConfig &config,
    const std::shared_ptr<PhysicsFactory> physicsFactory)
    : Connection(registry, world) {
  revoluteJoint = physicsFactory->createRevoluteJoint(config.templateJointCfg);
  registerChild(revoluteJoint);
}

const std::shared_ptr<RevoluteJoint>
RevoluteConnection::getRevoluteJoint() const {
  return revoluteJoint;
}

RevoluteConnectionConfig RevoluteConnectionConfig::defaultConfig() {
  RevoluteConnectionConfig ret;
  ret.templateJointCfg = RevoluteJointConfig::defaultConfig();
  return ret;
}
