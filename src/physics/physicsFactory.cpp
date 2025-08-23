#include "physicsFactory.hpp"
#include "physicsComponents.hpp"
#include "registryObjectFactory.hpp"
#include "revoluteJoint.hpp"

PhysicsFactory::PhysicsFactory(entt::registry &registry,
                               const std::shared_ptr<World> world)
    : RegistryObjectFactory(registry), world(world) {}

std::shared_ptr<Circle>
PhysicsFactory::createCircle(const CircleConfig &config) {
  std::shared_ptr<Circle> ret = nullptr;
  try {
    ret = std::shared_ptr<Circle>(new Circle(registry, *world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerShape(ret);
  return ret;
}

std::shared_ptr<Polygon>
PhysicsFactory::createPolygon(const PolygonConfig &config) {
  std::shared_ptr<Polygon> ret = nullptr;
  try {
    ret = std::shared_ptr<Polygon>(new Polygon(registry, *world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerShape(ret);
  return ret;
}

std::shared_ptr<Capsule>
PhysicsFactory::createCapsule(const CapsuleConfig &config) {
  std::shared_ptr<Capsule> ret = nullptr;
  try {
    ret = std::shared_ptr<Capsule>(new Capsule(registry, *world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerShape(ret);
  return ret;
}

std::shared_ptr<EmptyShape>
PhysicsFactory::createEmptyShape(const EmptyShapeConfig &config) {
  std::shared_ptr<EmptyShape> ret = nullptr;
  try {
    ret = std::shared_ptr<EmptyShape>(new EmptyShape(registry, *world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerShape(ret);
  return ret;
}

std::shared_ptr<Segment>
PhysicsFactory::createSegment(const SegmentConfig &config) {
  std::shared_ptr<Segment> ret = nullptr;
  try {
    ret = std::shared_ptr<Segment>(new Segment(registry, *world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerShape(ret);
  return ret;
}

std::shared_ptr<RevoluteJoint>
PhysicsFactory::createRevoluteJoint(const RevoluteJointConfig &config) {
  std::shared_ptr<RevoluteJoint> ret = nullptr;
  try {
    ret = std::shared_ptr<RevoluteJoint>(
        new RevoluteJoint(registry, *world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerJoint(ret);
  return ret;
}

std::shared_ptr<DistanceJoint>
PhysicsFactory::createDistanceJoint(const DistanceJointConfig &config) {
  std::shared_ptr<DistanceJoint> ret = nullptr;
  try {
    ret = std::shared_ptr<DistanceJoint>(
        new DistanceJoint(registry, *world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerJoint(ret);
  return ret;
}

std::shared_ptr<MouseJoint>
PhysicsFactory::createMouseJoint(const MouseJointConfig &config) {
  std::shared_ptr<MouseJoint> ret = nullptr;
  try {
    ret = std::shared_ptr<MouseJoint>(new MouseJoint(registry, *world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerJoint(ret);
  return ret;
}

std::shared_ptr<WeldJoint>
PhysicsFactory::createWeldJoint(const WeldJointConfig &config) {
  std::shared_ptr<WeldJoint> ret = nullptr;
  try {
    ret = std::shared_ptr<WeldJoint>(new WeldJoint(registry, *world, config));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerJoint(ret);
  return ret;
}

int PhysicsFactory::getNextNegativeId() {
  static int ret = -1;
  return ret--;
}

int PhysicsFactory::getNextPositiveId() {
  static int ret = 1;
  return ret++;
}

void PhysicsFactory::registerJoint(std::shared_ptr<Joint> joint) {
  auto ent = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsJoint>(ent);
  comp.joint = joint;
  joint->setEntity(ent);
}

void PhysicsFactory::registerShape(std::shared_ptr<Shape> shape) {
  auto ent = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsShape>(ent);
  comp.shape = shape;
  shape->setEntity(ent);
}