#include "connection.hpp"
#include "weldJoint.hpp"
#include "physicsFactory.hpp"

struct WeldConnectionConfig : ConnectionConfig {
  WeldJointConfig templateJointCfg;
  static WeldConnectionConfig defaultConfig();
};
class WeldConnection : public Connection {
public:
  const std::shared_ptr<WeldJoint> getWeldJoint() const;

protected:
  WeldConnection(entt::registry &registry,
                     const std::shared_ptr<World> world,
                     const WeldConnectionConfig &config,
                     const std::shared_ptr<PhysicsFactory> physicsFactory);
  std::shared_ptr<WeldJoint> weldJoint;

  friend class ConnectionFactory;
};