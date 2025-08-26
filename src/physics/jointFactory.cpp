#include "jointFactory.hpp"
#include "physicsComponents.hpp"
#include "registryObjectFactory.hpp"
#include "revoluteJoint.hpp"

JointFactory::JointFactory(entt::registry &registry,
                               const std::shared_ptr<World> world)
    : RegistryObjectFactory(registry), world(world) {}


std::shared_ptr<RevoluteJoint>
JointFactory::createRevoluteJoint(const RevoluteJointConfig &config) {
  return create<RevoluteJoint>(config);
}

std::shared_ptr<DistanceJoint>
JointFactory::createDistanceJoint(const DistanceJointConfig &config) {
  return create<DistanceJoint>(config);
}

std::shared_ptr<MouseJoint>
JointFactory::createMouseJoint(const MouseJointConfig &config) {
  return create<MouseJoint>(config);
}

std::shared_ptr<WeldJoint>
JointFactory::createWeldJoint(const WeldJointConfig &config) {
  return create<WeldJoint>(config);
}

