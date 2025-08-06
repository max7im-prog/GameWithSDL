#include "bodyFactory.hpp"
#include "physicsComponents.hpp"
#include "physicsFactory.hpp"
BodyFactory::BodyFactory(entt::registry &registry,
                         const std::shared_ptr<World> world,
                         const std::shared_ptr<PhysicsFactory> physicsFactory)
    : registry(registry), world(world), physicsFactory(physicsFactory) {}

std::shared_ptr<CircleBody>
BodyFactory::createCircleBody(const CircleBodyConfig &config) {
  std::shared_ptr<CircleBody> ret = nullptr;
  try {
    ret = std::shared_ptr<CircleBody>(
        new CircleBody(registry, world, config, physicsFactory));
  } catch (std::exception &e) {
    return nullptr;
  }
  ret->entity = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsBodyPart>(ret->entity);
  comp.bodyPart = ret;
  return ret;
}