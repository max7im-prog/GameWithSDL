#pragma once
#include "joint.hpp"
#include "physicsFactory.hpp"
#include "shape.hpp"

struct BodyPartConfig;

class BodyPart {
public:
  virtual void update([[maybe_unused]] float dt) {};
  entt::entity getEntity();
  virtual ~BodyPart() = default;

protected:
  BodyPart(entt::registry &registry );
  BodyPart() = delete;
  BodyPart(BodyPart &other) = delete;
  BodyPart(BodyPart &&other) = delete;
  BodyPart operator=(BodyPart &other) = delete;
  BodyPart operator=(BodyPart &&other) = delete;

  entt::entity entity = entt::null;
  std::vector<std::shared_ptr<Joint>> joints;
  std::vector<std::shared_ptr<Shape>> shapes;
  entt::registry &registry;

  friend class PhysicsFactory;
};