#pragma once

#include "objectConfig.hpp"
#include "registryComposite.hpp"
#include "visitor.hpp"
#include "world.hpp"

struct ConnectionConfig :public ObjectConfig{};

class Connection : public RegistryComposite, public virtual IVisitable {
public:
  virtual ~Connection();

protected:
  Connection(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> world;

private:
  friend class ConnectionFactory;
};