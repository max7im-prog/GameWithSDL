#pragma once

#include "objectConfig.hpp"
#include "registryComposite.hpp"
#include "visitor.hpp"
#include "world.hpp"
#include "shape.hpp"
#include "joint.hpp"

struct ConnectionConfig :public ObjectConfig{};

class Connection : public RegistryComposite, public virtual IVisitable {
public:
  virtual ~Connection();

  const std::unordered_map<std::string, std::weak_ptr<Shape>> &
  getShapes() const;
  const std::unordered_map<std::string, std::weak_ptr<Joint>> &
  getJoints() const;

  void registerShape(std::weak_ptr<Shape> body, const std::string &name);
  void registerJoint(std::weak_ptr<Joint> connection, const std::string &name);

protected:
  Connection(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> world;

  std::unordered_map<std::string, std::weak_ptr<Shape>> _shapes;
  std::unordered_map<std::string, std::weak_ptr<Joint>> _joints;

private:
  friend class ConnectionFactory;
};