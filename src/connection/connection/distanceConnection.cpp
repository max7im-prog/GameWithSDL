#include "distanceConnection.hpp"
#include "distanceJoint.hpp"

DistanceConnection::DistanceConnection(
    entt::registry &registry, const std::shared_ptr<World> world,
    const DistanceConnectionConfig &config,
    const std::shared_ptr<ShapeFactory> shapeFactory,
    const std::shared_ptr<JointFactory> jointFactory)
    : Connection(registry, world) {
  distanceJoint = jointFactory->create<DistanceJoint>(config.templateJointCfg);
  registerChild(distanceJoint);
}

const std::shared_ptr<DistanceJoint>
DistanceConnection::getDistanceJoint() const {
  auto ret = distanceJoint.lock();
  if (!ret)
    throw std::runtime_error("Joint expired");
  return ret;
}

void DistanceConnectionConfig::defaultConfig() {
  templateJointCfg.defaultConfig();
}

void SegmentConfig::fromJSON(const nlohmann::json &json) {
  // TODO: implement
}
