#include "distanceConnection.hpp"
#include "body.hpp"
#include "distanceJoint.hpp"

DistanceConnection::DistanceConnection(
    entt::registry &registry, const std::shared_ptr<World> world,
    const DistanceConnection::Config &config,
    const std::shared_ptr<ShapeFactory> shapeFactory,
    const std::shared_ptr<JointFactory> jointFactory)
    : Connection(registry, world) {

  auto cfg = config.templateJointCfg;
  auto bodyA = config._attachA._body.lock();
  auto bodyB = config._attachB._body.lock();
  if (!bodyA) {
    spdlog::error("Distanceconnection - body A expired");
    throw std::runtime_error("Distanceconnection: body expired");
  }
  if (!bodyB) {
    spdlog::error("Distanceconnection - body B expired");
    throw std::runtime_error("Distanceconnection: body expired");
  }
  std::shared_ptr<Shape> shapeA;
  try {
    shapeA = bodyA->getShapes().at(config._attachA._shapeName).lock();
  } catch (const std::exception &e) {
    spdlog::error("No shape named '{}' in body A", config._attachA._shapeName);
    throw;
  }

  std::shared_ptr<Shape> shapeB;
  try {
    shapeB = bodyB->getShapes().at(config._attachB._shapeName).lock();
  } catch (const std::exception &e) {
    spdlog::error("No shape named '{}' in body B", config._attachB._shapeName);
    throw;
  }
  if (!shapeA) {
    spdlog::error("Distanceconnection, bodyA - Shape '{}' expired",
                  config._attachA._shapeName);
    throw std::runtime_error("Distanceconnection - bodyA: shape expired'" +
                             config._attachA._shapeName + "'");
  }
  if (!shapeB) {
    spdlog::error("Distanceconnection, bodyB - Shape '{}' expired",
                  config._attachB._shapeName);
    throw std::runtime_error("Distanceconnection - bodyB: shape expired'" +
                             config._attachA._shapeName + "'");
  }

  cfg.jointDef.bodyIdA = shapeA->getBodyId();
  cfg.jointDef.bodyIdB = shapeB->getBodyId();
  cfg.jointDef.localAnchorA = config._attachA._shapeLocalPoint;
  cfg.jointDef.localAnchorB = config._attachB._shapeLocalPoint;

  distanceJoint = jointFactory->create<DistanceJoint>(config.templateJointCfg);
  registerJoint(distanceJoint, JointNames::s_main);
}

const std::shared_ptr<DistanceJoint>
DistanceConnection::getDistanceJoint() const {
  auto ret = distanceJoint.lock();
  if (!ret)
    throw std::runtime_error("Joint expired");
  return ret;
}

void DistanceConnection::Config::defaultConfig() {
  templateJointCfg.defaultConfig();
}
