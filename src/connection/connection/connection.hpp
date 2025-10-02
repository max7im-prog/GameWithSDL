#pragma once



#include "registryComposite.hpp"
#include "visitor.hpp"
#include "world.hpp"

struct ConnectionConfig {};

class Connection : public RegistryComposite , public SceneNode{
public:
  virtual ~Connection();

  virtual void accept(Visitor &v) override;
protected:
  Connection(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> world;

private:
  friend class ConnectionFactory;
};