#pragma once
#include "connection.hpp"
#include "jointFactory.hpp"
#include "revoluteJoint.hpp"
#include "shapeFactory.hpp"

struct RevoluteConnectionConfig : ConnectionConfig {
  RevoluteJointConfig templateJointCfg;
  static RevoluteConnectionConfig defaultConfig();
};
class RevoluteConnection : public Connection {
public:
  using Config = RevoluteConnectionConfig;
  const std::weak_ptr<RevoluteJoint> getRevoluteJoint() const;

protected:
  RevoluteConnection(entt::registry &registry,
                     const std::shared_ptr<World> world,
                     const RevoluteConnectionConfig &config,
                     const std::shared_ptr<ShapeFactory> shapeFactory,
                     const std::shared_ptr<JointFactory> jointFactory);
  std::weak_ptr<RevoluteJoint> revoluteJoint;

  friend class ConnectionFactory;
};