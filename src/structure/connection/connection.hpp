#pragma once

#include "structure.hpp"
#include "visitor.hpp"
#include "world.hpp"

class Connection : public Structure, public virtual IVisitable {
public:
  struct Config : public Structure::Config {};
  virtual ~Connection();

protected:
  Connection(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> world;

private:
  friend class ConnectionFactory;
};
