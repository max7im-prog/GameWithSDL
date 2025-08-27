#pragma once



#include "registryComposite.hpp"
#include "world.hpp"

struct ConnectionConfig {};

class Connection : public RegistryComposite {
public:
  virtual void update([[maybe_unused]] float dt) override {};
  virtual ~Connection();

protected:
  Connection(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> world;

private:
  friend class ConnectionFactory;
};