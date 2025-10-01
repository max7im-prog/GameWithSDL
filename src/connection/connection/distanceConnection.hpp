#include "connection.hpp"
#include "distanceJoint.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"

struct DistanceConnectionConfig : public ConnectionConfig {
  DistanceJointConfig templateJointCfg;
  static DistanceConnectionConfig defaultConfig();
};
class DistanceConnection : public Connection {
public:
  using Config = DistanceConnectionConfig;
  const std::shared_ptr<DistanceJoint> getDistanceJoint() const;

protected:
  DistanceConnection(entt::registry &registry,
                     const std::shared_ptr<World> world,
                     const DistanceConnectionConfig &config,
                     const std::shared_ptr<ShapeFactory> shapeFactory,
                     const std::shared_ptr<JointFactory> jointFactory);
  std::weak_ptr<DistanceJoint> distanceJoint;

  friend class ConnectionFactory;
};