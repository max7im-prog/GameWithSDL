#include "connection.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"
#include "weldJoint.hpp"

struct WeldConnectionConfig : ConnectionConfig {
  WeldJointConfig templateJointCfg;
  static WeldConnectionConfig defaultConfig();
};
class WeldConnection : public Connection {
public:
  using Config = WeldConnectionConfig;
  const std::shared_ptr<WeldJoint> getWeldJoint() const;

protected:
  WeldConnection(entt::registry &registry, const std::shared_ptr<World> world,
                 const WeldConnectionConfig &config,
                 const std::shared_ptr<ShapeFactory> shapeFactory,
                 const std::shared_ptr<JointFactory> jointFactory);
  std::shared_ptr<WeldJoint> weldJoint;

  friend class ConnectionFactory;
};