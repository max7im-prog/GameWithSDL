#pragma once
#include "connection.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"
#include "weldJoint.hpp"

struct WeldConnectionConfig : ConnectionConfig {
  WeldJointConfig templateJointCfg;
  void defaultConfig() override;
};
class WeldConnection : public Connection, public VisitableImpl<WeldConnection> {
public:
  using Config = WeldConnectionConfig;
  const std::shared_ptr<WeldJoint> getWeldJoint() const;

protected:
  WeldConnection(entt::registry &registry, const std::shared_ptr<World> world,
                 const WeldConnectionConfig &config,
                 const std::shared_ptr<ShapeFactory> shapeFactory,
                 const std::shared_ptr<JointFactory> jointFactory);
  std::weak_ptr<WeldJoint> weldJoint;

  friend class ConnectionFactory;
};