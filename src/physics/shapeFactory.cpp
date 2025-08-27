#include "shapeFactory.hpp"
#include "circle.hpp"

ShapeFactory::ShapeFactory(entt::registry &registry,
                           const std::shared_ptr<World> world)
    : RegistryObjectFactory(registry), world(world) {}

int ShapeFactory::getNextNegativeId() {
  static int ret = -1;
  return ret--;
}

int ShapeFactory::getNextPositiveId() {
  static int ret = 1;
  return ret++;
}