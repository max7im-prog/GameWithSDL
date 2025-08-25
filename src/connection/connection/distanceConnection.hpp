#include "connection.hpp"
#include "distanceJoint.hpp"
#include "physicsFactory.hpp"

struct DistanceConnectionConfig : ConnectionConfig {
  DistanceJointConfig templateJointCfg;
  DistanceConnectionConfig defaultConfig();
};
class DistanceConnection : public Connection {
public:
  const std::shared_ptr<DistanceJoint> getDistanceJoint() const;

protected:
  DistanceConnection(entt::registry &registry,
                     const std::shared_ptr<World> world,
                     const DistanceConnectionConfig &config,
                     const std::shared_ptr<PhysicsFactory> physicsFactory);
  std::shared_ptr<DistanceJoint> distanceJoint;

  friend class ConnectionFactory;
};