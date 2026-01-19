#include "bodyFactory.hpp"
#include "registryObjectFactory.hpp"

BodyFactory::BodyFactory(entt::registry &registry,
                         const std::shared_ptr<World> world,
                         const std::shared_ptr<ShapeFactory> shapeFactory,
                         const std::shared_ptr<JointFactory> jointFactory)
    : RegistryObjectFactory(registry), world(world), shapeFactory(shapeFactory),
      jointFactory(jointFactory) {}
