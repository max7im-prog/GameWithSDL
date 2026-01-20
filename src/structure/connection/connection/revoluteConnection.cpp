#include "revoluteConnection.hpp"
#include "body.hpp"
#include "revoluteJoint.hpp"
#include "spdlog/spdlog.h"
#include <stdexcept>

RevoluteConnection::RevoluteConnection(
    entt::registry &registry, const std::shared_ptr<World> world,
    const RevoluteConnection::Config &config,
    const std::shared_ptr<ShapeFactory> shapeFactory,
    const std::shared_ptr<JointFactory> jointFactory)
    : Connection(registry, world) {
  auto cfg = config.templateJointCfg;
  auto bodyA = config._attachA._body.lock();
  auto bodyB = config._attachB._body.lock();
  if (!bodyA) {
    spdlog::error("RevoluteConnection - body A expired");
    throw std::runtime_error("RevoluteConnection: body expired");
  }
  if (!bodyB) {
    spdlog::error("RevoluteConnection - body B expired");
    throw std::runtime_error("RevoluteConnection: body expired");
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
    spdlog::error("RevoulteConnection, bodyA - Shape '{}' expired",
                  config._attachA._shapeName);
    throw std::runtime_error("RevoluteConnection - bodyA: shape expired'" +
                             config._attachA._shapeName + "'");
  }
  if (!shapeB) {
    spdlog::error("RevoulteConnection, bodyB - Shape '{}' expired",
                  config._attachB._shapeName);
    throw std::runtime_error("RevoluteConnection - bodyB: shape expired'" +
                             config._attachA._shapeName + "'");
  }

  cfg.jointDef.bodyIdA = shapeA->getBodyId();
  cfg.jointDef.bodyIdB = shapeB->getBodyId();
  cfg.jointDef.localAnchorA = config._attachA._shapeLocalPoint;
  cfg.jointDef.localAnchorB = config._attachB._shapeLocalPoint;

  revoluteJoint = jointFactory->create<RevoluteJoint>(cfg);
  registerJoint(revoluteJoint, "main");
}

const std::shared_ptr<RevoluteJoint>
RevoluteConnection::getRevoluteJoint() const {
  auto ret = revoluteJoint.lock();
  if (!ret)
    throw std::runtime_error("Joint expired");
  return ret;
}

void RevoluteConnection::Config::defaultConfig() {
  templateJointCfg.defaultConfig();
}
