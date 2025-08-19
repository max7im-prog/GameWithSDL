#include "bodyFactory.hpp"
#include "bodyComponents.hpp"
#include "physicsFactory.hpp"
#include "registryObjectFactory.hpp"
BodyFactory::BodyFactory(entt::registry &registry,
                         const std::shared_ptr<World> world,
                         const std::shared_ptr<PhysicsFactory> physicsFactory)
    : RegistryObjectFactory(registry), world(world),
      physicsFactory(physicsFactory) {}

std::shared_ptr<CircleBody>
BodyFactory::createCircleBody(const CircleBodyConfig &config) {
  std::shared_ptr<CircleBody> ret = nullptr;
  try {
    ret = std::shared_ptr<CircleBody>(
        new CircleBody(registry, world, config, physicsFactory));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerBody(ret);
  return ret;
}

std::shared_ptr<CapsuleBody>
BodyFactory::createCapsuleBody(const CapsuleBodyConfig &config) {
  std::shared_ptr<CapsuleBody> ret = nullptr;
  try {
    ret = std::shared_ptr<CapsuleBody>(
        new CapsuleBody(registry, world, config, physicsFactory));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerBody(ret);
  return ret;
}

std::shared_ptr<PolygonBody>
BodyFactory::createPolygonBody(const PolygonBodyConfig &config) {
  std::shared_ptr<PolygonBody> ret = nullptr;
  try {
    ret = std::shared_ptr<PolygonBody>(
        new PolygonBody(registry, world, config, physicsFactory));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerBody(ret);
  return ret;
}

std::shared_ptr<LimbBody>
BodyFactory::createLimbBody(const LimbBodyConfig &config) {
  std::shared_ptr<LimbBody> ret = nullptr;
  try {
    ret = std::shared_ptr<LimbBody>(
        new LimbBody(registry, world, config, physicsFactory));
  } catch (std::exception &e) {
    return nullptr;
  }
  registerBody(ret);
  return ret;
}

void BodyFactory::registerBody(std::shared_ptr<BodyPart> body) {
  auto ent = registry.create();
  auto &comp = registry.emplace_or_replace<PhysicsBodyPart>(ent);
  comp.bodyPart = body;
  body->setEntity(ent);
}