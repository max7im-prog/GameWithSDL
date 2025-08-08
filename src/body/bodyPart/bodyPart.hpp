#pragma once
#include "box2d/box2d.h"
#include "joint.hpp"
#include "physicsFactory.hpp"
#include "shape.hpp"

struct BodyPartConfig {};

class BodyPart {
public:
  virtual void update([[maybe_unused]] float dt) {};
  entt::entity getEntity();
  virtual ~BodyPart();

  // Removes a BodyPart from the registry if registered. Does not automatically
  // delete the shape as shared pointers are responsible for deletion.
  void remove();

protected:
  BodyPart(entt::registry &registry, const std::shared_ptr<World> world);
  const std::shared_ptr<World> world;

  // Registers shape in shapes of this BodyPart. Has to be called for every
  // shape BodyPart consists of. Otherwise you are going to see issues with
  // deletiion and removal of BodyParts.
  void registerShape(std::shared_ptr<Shape> shape);

  // Registers joint in joints of this BodyPart. Has to be called for every
  // joint BodyPart consists of. Otherwise you are going to see issues with
  // deletiion and removal of BodyParts.
  void registerJoint(std::shared_ptr<Joint> joint);

private:
  std::set<std::shared_ptr<Shape>> shapes;
  std::set<std::shared_ptr<Joint>> joints;

  BodyPart() = delete;
  BodyPart(BodyPart &other) = delete;
  BodyPart(BodyPart &&other) = delete;
  BodyPart operator=(BodyPart &other) = delete;
  BodyPart operator=(BodyPart &&other) = delete;

  entt::entity entity = entt::null;
  entt::registry &registry;
  friend class BodyFactory;
};