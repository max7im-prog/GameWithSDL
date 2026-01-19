#pragma once
#include "pseudo3d.hpp"
#include "structure.hpp"

class Body : public Structure, public virtual IVisitable, public Pseudo3d {
public:
  struct Config : public Structure::Config {};

  virtual void update([[maybe_unused]] float dt) override {};
  virtual ~Body();
  virtual b2Vec2 getWorldPos() = 0;

protected:
  Body(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> world;

  virtual void performRotation(b2Rot rot) override;

private:
  friend class BodyFactory;
};
