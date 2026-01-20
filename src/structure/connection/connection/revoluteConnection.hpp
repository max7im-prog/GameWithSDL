#pragma once
#include "connection.hpp"
#include "jointFactory.hpp"
#include "revoluteJoint.hpp"
#include "shapeFactory.hpp"

class RevoluteConnection : public Connection,
                           public VisitableImpl<RevoluteConnection> {
public:
  struct Config : Connection::Config {
    RevoluteJoint::Config templateJointCfg;
    void defaultConfig() override;
    BodyAttach _attachA;
    BodyAttach _attachB;
  };
  const std::shared_ptr<RevoluteJoint> getRevoluteJoint() const;

protected:
  RevoluteConnection(entt::registry &registry,
                     const std::shared_ptr<World> world,
                     const RevoluteConnection::Config &config,
                     const std::shared_ptr<ShapeFactory> shapeFactory,
                     const std::shared_ptr<JointFactory> jointFactory);
  std::weak_ptr<RevoluteJoint> revoluteJoint;

  friend class ConnectionFactory;
};
