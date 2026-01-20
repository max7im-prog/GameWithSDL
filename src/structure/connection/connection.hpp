#pragma once

#include "structure.hpp"
#include "visitor.hpp"
#include "world.hpp"

class Connection : public Structure, public virtual IVisitable {
public:
  struct Config : public Structure::Config {
    struct BodyAttach {
      std::weak_ptr<Body> _body;
      std::string _shapeName = "";
      b2Vec2 _shapeLocalPoint = {0, 0};
    };
  };
  virtual ~Connection() = 0;

protected:
  Connection(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> _world;

private:
  friend class ConnectionFactory;
};
