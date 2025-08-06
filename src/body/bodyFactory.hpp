#pragma once
#include "physicsFactory.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
class BodyFactory {
public:
  BodyFactory(entt::registry &registry, const std::shared_ptr<World> world,
              const std::shared_ptr<PhysicsFactory> physicsFactory);

private:
  BodyFactory() = delete;
  BodyFactory(BodyFactory &other) = delete;
  BodyFactory(BodyFactory &&other) = delete;
  BodyFactory operator=(BodyFactory &other) = delete;
  BodyFactory operator=(BodyFactory &&other) = delete;

  entt::registry &registry;
  const std::shared_ptr<World> world;
  const std::shared_ptr<PhysicsFactory> physicsFactory;
};