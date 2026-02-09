#pragma once
#include "connection.hpp"
#include "distanceJoint.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"

class DistanceConnection : public Connection,
                           public VisitableImpl<DistanceConnection> {
public:
  struct Config : public Connection::Config {
    DistanceJoint::Config templateJointCfg;
    void defaultConfig() override;
    BodyAttach _attachA;
    BodyAttach _attachB;
  };
  struct JointNames {
    static constexpr std::string_view s_main = "main";

  private:
    ~JointNames() = delete;
  };

  const std::shared_ptr<DistanceJoint> getDistanceJoint() const;

protected:
  DistanceConnection(entt::registry &registry,
                     const std::shared_ptr<World> world,
                     const DistanceConnection::Config &config,
                     const std::shared_ptr<ShapeFactory> shapeFactory,
                     const std::shared_ptr<JointFactory> jointFactory);
  std::weak_ptr<DistanceJoint> distanceJoint;

  friend class ConnectionFactory;
};
