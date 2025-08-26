#include "bodyFactory.hpp"
#include "registryObjectFactory.hpp"
#include "segmentBody.hpp"
BodyFactory::BodyFactory(entt::registry &registry,
                         const std::shared_ptr<World> world,
                         const std::shared_ptr<ShapeFactory> shapeFactory,
                         const std::shared_ptr<JointFactory> jointFactory)
    : RegistryObjectFactory(registry), world(world), shapeFactory(shapeFactory),
      jointFactory(jointFactory) {}

std::shared_ptr<CircleBody>
BodyFactory::createCircleBody(const CircleBodyConfig &config) {
  return create<CircleBody>(config);
}

std::shared_ptr<CapsuleBody>
BodyFactory::createCapsuleBody(const CapsuleBodyConfig &config) {
  return create<CapsuleBody>(config);
}

std::shared_ptr<PolygonBody>
BodyFactory::createPolygonBody(const PolygonBodyConfig &config) {
  return create<PolygonBody>(config);
}

std::shared_ptr<LimbBody>
BodyFactory::createLimbBody(const LimbBodyConfig &config) {
  return create<LimbBody>(config);
}

std::shared_ptr<SegmentBody>
BodyFactory::createSegmentBody(const SegmentBodyConfig &config) {
  return create<SegmentBody>(config);
}
