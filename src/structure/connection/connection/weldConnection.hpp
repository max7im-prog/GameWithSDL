#pragma once
#include "connection.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"
#include "weldJoint.hpp"

class WeldConnection : public Connection, public VisitableImpl<WeldConnection> {
public:
  struct Config : Connection::Config {
    WeldJoint::Config templateJointCfg;
    void defaultConfig() override;
    BodyAttach _attachA;
    BodyAttach _attachB;
  };
  const std::shared_ptr<WeldJoint> getWeldJoint() const;

protected:
  WeldConnection(entt::registry &registry, const std::shared_ptr<World> world,
                 const WeldConnection::Config &config,
                 const std::shared_ptr<ShapeFactory> shapeFactory,
                 const std::shared_ptr<JointFactory> jointFactory);
  std::weak_ptr<WeldJoint> weldJoint;

  friend class ConnectionFactory;
};
