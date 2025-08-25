#include "connection.hpp"
#include "revoluteJoint.hpp"
#include "physicsFactory.hpp"

struct RevoluteConnectionConfig : ConnectionConfig {
  RevoluteJointConfig templateJointCfg;
  static RevoluteConnectionConfig defaultConfig();
};
class RevoluteConnection : public Connection {
public:
  const std::shared_ptr<RevoluteJoint> getRevoluteJoint() const;

protected:
  RevoluteConnection(entt::registry &registry,
                     const std::shared_ptr<World> world,
                     const RevoluteConnectionConfig &config,
                     const std::shared_ptr<PhysicsFactory> physicsFactory);
  std::shared_ptr<RevoluteJoint> revoluteJoint;

  friend class ConnectionFactory;
};