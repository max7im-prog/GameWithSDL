#include "distanceConnection.hpp"
#include "distanceJoint.hpp"

DistanceConnection::DistanceConnection(
    entt::registry &registry, const std::shared_ptr<World> world,
    const DistanceConnectionConfig &config,
    const std::shared_ptr<ShapeFactory> shapeFactory,
    const std::shared_ptr<JointFactory> jointFactory)
    : Connection(registry, world) {
  distanceJoint = jointFactory->createDistanceJoint(config.templateJointCfg);
  registerChild(distanceJoint);
}

const std::shared_ptr<DistanceJoint>
DistanceConnection::getDistanceJoint() const {
  return distanceJoint;
}

DistanceConnectionConfig DistanceConnectionConfig::defaultConfig() {
  DistanceConnectionConfig ret;
  ret.templateJointCfg = DistanceJointConfig::defaultConfig();
  return ret;
}
