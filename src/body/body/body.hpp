#pragma once
#include "box2d/box2d.h"
#include "joint.hpp"
#include "objectConfig.hpp"
#include "registryComposite.hpp"
#include "shape.hpp"

struct BodyConfig : public ObjectConfig {};

class Body : public RegistryComposite, public virtual IVisitable {
public:
  virtual void update([[maybe_unused]] float dt) override {};
  virtual ~Body();
  virtual b2Vec2 getWorldPos() = 0;

  const std::unordered_map<std::string, std::weak_ptr<Shape>> &
  getShapes() const;
  const std::unordered_map<std::string, std::weak_ptr<Joint>> &
  getJoints() const;

  void registerShape(std::weak_ptr<Shape> body, const std::string &name);
  void registerJoint(std::weak_ptr<Joint> connection, const std::string &name);

protected:
  Body(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> world;

  std::unordered_map<std::string, std::weak_ptr<Shape>> _shapes;
  std::unordered_map<std::string, std::weak_ptr<Joint>> _joints;

private:
  friend class BodyFactory;
};