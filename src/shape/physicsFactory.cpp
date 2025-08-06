#include "physicsFactory.hpp"
#include "physicsComponents.hpp"
#include "revoluteJoint.hpp"

PhysicsFactory::PhysicsFactory(entt::registry &registry,
                               const std::shared_ptr<World> world)
    : registry(registry), world(world) {}

std::shared_ptr<Circle> PhysicsFactory::createCircle(CircleConfig config) {
  std::shared_ptr<Circle> ret = nullptr;
  try {
    ret = std::shared_ptr<Circle>(new Circle(registry,*world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  ret->entity = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsBody>(ret->entity);
  comp.shape = ret;
  return ret;
}

std::shared_ptr<Polygon> PhysicsFactory::createPolygon(PolygonConfig config) {
  std::shared_ptr<Polygon> ret = nullptr;
  try {
    ret = std::shared_ptr<Polygon>(new Polygon(registry,*world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  ret->entity = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsBody>(ret->entity);
  comp.shape = ret;
  return ret;
}

std::shared_ptr<Capsule> PhysicsFactory::createCapsule(CapsuleConfig config) {
  std::shared_ptr<Capsule> ret = nullptr;
  try {
    ret = std::shared_ptr<Capsule>(new Capsule(registry,*world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  ret->entity = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsBody>(ret->entity);
  comp.shape = ret;
  return ret;
}

std::shared_ptr<RevoluteJoint>
PhysicsFactory::createRevoluteJoint(RevoluteJointConfig config) {
    std::shared_ptr<RevoluteJoint> ret = nullptr;
  try {
    ret = std::shared_ptr<RevoluteJoint>(new RevoluteJoint(registry,*world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  ret->entity = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsJoint>(ret->entity);
  comp.joint = ret;
  return ret;
}

std::shared_ptr<DistanceJoint>
PhysicsFactory::createDistanceJoint(DistanceJointConfig config) {
    std::shared_ptr<DistanceJoint> ret = nullptr;
  try {
    ret = std::shared_ptr<DistanceJoint>(new DistanceJoint(registry,*world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  ret->entity = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsJoint>(ret->entity);
  comp.joint = ret;
  return ret;
}