#pragma once
#include "basicWorld.hpp"
#include "bodyFactory.hpp"
#include "connectionFactory.hpp"
#include "entt/entity/fwd.hpp"
#include "game.hpp"
#include "jointFactory.hpp"
#include "shapeFactory.hpp"
#include "terrainFactory.hpp"
#include "worldFactory.hpp"
#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <memory>

// Base fixture: sets up registry + all factories
class WorldRegistryTest : public ::testing::Test {
public:
  static bool registryIsEmpty(entt::registry &registry) {
    for (auto ent : registry.view<entt::entity>()) {
      return false;
    }
    return true;
  };

  static size_t registrySize(entt::registry &registry) {
    size_t s = 0;
    for (auto ent : registry.view<entt::entity>()) {
      s++;
    }
    return s;
  };

protected:
  template <typename F> std::shared_ptr<F> getFactory();

  entt::registry registry;
  std::shared_ptr<World> world;
  std::shared_ptr<WorldFactory> worldFactory;
  std::shared_ptr<ShapeFactory> shapeFactory;
  std::shared_ptr<JointFactory> jointFactory;
  std::shared_ptr<BodyFactory> bodyFactory;
  std::shared_ptr<ConnectionFactory> connectionFactory;
  std::shared_ptr<CreatureFactory> creatureFactory;
  std::shared_ptr<TerrainFactory> terrainFactory;

  void initFactories() {

    worldFactory = std::make_shared<WorldFactory>(registry);
    {
      auto config = BasicWorld::Config::defaultConfig();
      world = worldFactory->create<BasicWorld>(config);
    }

    shapeFactory = std::make_shared<ShapeFactory>(registry, world);
    jointFactory = std::make_shared<JointFactory>(registry, world);
    bodyFactory = std::make_shared<BodyFactory>(registry, world, shapeFactory,
                                                jointFactory);
    connectionFactory = std::make_shared<ConnectionFactory>(
        registry, world, shapeFactory, jointFactory);
    creatureFactory = std::make_shared<CreatureFactory>(
        registry, world, bodyFactory, connectionFactory);
    terrainFactory = std::make_shared<TerrainFactory>(
        registry, world, bodyFactory, connectionFactory);
  }

  void deinitFactories() {
    worldFactory.reset();
    shapeFactory.reset();
    jointFactory.reset();
    bodyFactory.reset();
    connectionFactory.reset();
    creatureFactory.reset();
    terrainFactory.reset();
  }
};
template <>
inline std::shared_ptr<WorldFactory>
WorldRegistryTest::getFactory<WorldFactory>() {
  return worldFactory;
}

template <>
inline std::shared_ptr<ShapeFactory>
WorldRegistryTest::getFactory<ShapeFactory>() {
  return shapeFactory;
}

template <>
inline std::shared_ptr<JointFactory>
WorldRegistryTest::getFactory<JointFactory>() {
  return jointFactory;
}

template <>
inline std::shared_ptr<BodyFactory>
WorldRegistryTest::getFactory<BodyFactory>() {
  return bodyFactory;
}

template <>
inline std::shared_ptr<ConnectionFactory>
WorldRegistryTest::getFactory<ConnectionFactory>() {
  return connectionFactory;
}

template <>
inline std::shared_ptr<CreatureFactory>
WorldRegistryTest::getFactory<CreatureFactory>() {
  return creatureFactory;
}

template <>
inline std::shared_ptr<TerrainFactory>
WorldRegistryTest::getFactory<TerrainFactory>() {
  return terrainFactory;
}
