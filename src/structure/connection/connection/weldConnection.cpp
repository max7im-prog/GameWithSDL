#include "weldConnection.hpp"
#include "body.hpp"
#include "weldJoint.hpp"

WeldConnection::WeldConnection(entt::registry &registry,
                               const std::shared_ptr<World> world,
                               const WeldConnection::Config &config,
                               const std::shared_ptr<ShapeFactory> shapeFactory,
                               const std::shared_ptr<JointFactory> jointFactory)
    : Connection(registry, world) {

  auto cfg = config.templateJointCfg;
  auto bodyA = config._attachA._body.lock();
  auto bodyB = config._attachB._body.lock();
  if (!bodyA) {
    spdlog::error("WeldConnection - body A expired");
    throw std::runtime_error("WeldConnection: body expired");
  }
  if (!bodyB) {
    spdlog::error("WeldConnection - body B expired");
    throw std::runtime_error("WeldConnection: body expired");
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
    spdlog::error("WeldConnection, bodyA - Shape '{}' expired",
                  config._attachA._shapeName);
    throw std::runtime_error("WeldConnection - bodyA: shape expired'" +
                             config._attachA._shapeName + "'");
  }
  if (!shapeB) {
    spdlog::error("WeldConnection, bodyB - Shape '{}' expired",
                  config._attachB._shapeName);
    throw std::runtime_error("WeldConnection - bodyB: shape expired'" +
                             config._attachA._shapeName + "'");
  }

  cfg.jointDef.bodyIdA = shapeA->getBodyId();
  cfg.jointDef.bodyIdB = shapeB->getBodyId();
  cfg.jointDef.localAnchorA = config._attachA._shapeLocalPoint;
  cfg.jointDef.localAnchorB = config._attachB._shapeLocalPoint;

  weldJoint = jointFactory->create<WeldJoint>(cfg);
  registerJoint(weldJoint, JointNames::s_main);
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
