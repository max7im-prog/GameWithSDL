
#include "basicWorld.hpp"
#include "bodyComponents.hpp"
#include "bodyFactory.hpp"
#include "box2d/types.h"
#include "capsuleBody.hpp"
#include "connectionFactory.hpp"
#include "creatureComponents.hpp"
#include "demoCreature.hpp"
#include "entt/entity/fwd.hpp"
#include "game.hpp"
#include "jointFactory.hpp"
#include "limbBody.hpp"
#include "physicsComponents.hpp"
#include "polygon.hpp"
#include "polygonBody.hpp"
#include "segment.hpp"
#include "shapeFactory.hpp"
#include "terrainFactory.hpp"
#include "worldFactory.hpp"
#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <memory>

bool registryIsEmpty(entt::registry &registry) {
  for (auto ent : registry.view<entt::entity>()) {
    return false;
  }
  return true;
};

size_t registrySize(entt::registry &registry) {
  size_t s = 0;
  for (auto ent : registry.view<entt::entity>()) {
    s++;
  }
  return s;
};

// Base fixture: sets up registry + all factories
class WorldRegistryTest : public ::testing::Test {
protected:
  static void SetUpTestSuite() {} // <- corrected name
  static void TearDownTestSuite() {}

  void SetUp() override { initFactories(); }
  void TearDown() override {
    world.reset();
    registry.clear();
    deinitFactories();
  }

  template <typename F> std::shared_ptr<F> getFactory();

protected: // <- make registry and factories visible to derived tests
  entt::registry registry;
  std::shared_ptr<World> world;
  std::shared_ptr<WorldFactory> worldFactory;
  std::shared_ptr<ShapeFactory> shapeFactory;
  std::shared_ptr<JointFactory> jointFactory;
  std::shared_ptr<BodyFactory> bodyFactory;
  std::shared_ptr<ConnectionFactory> connectionFactory;
  std::shared_ptr<CreatureFactory> creatureFactory;
  std::shared_ptr<TerrainFactory> terrainFactory;

private:
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
// Traits: bind Factory, Object, and Component together
template <typename TFactory, typename TObject, typename TComponent>
struct RegistryObjectTraits {
  using Factory = TFactory;
  using Object = TObject;
  using Component = TComponent;
};

// Fixture that uses traits
template <typename Traits> class RegistryObjectTest : public WorldRegistryTest {
public:
  using Factory = typename Traits::Factory;
  using Object = typename Traits::Object;
  using Component = typename Traits::Component;
};

// List of (Factory, Object, Component) triples to test
using TestTypes = ::testing::Types<

    RegistryObjectTraits<ShapeFactory, Circle, PhysicsShape>,
    RegistryObjectTraits<ShapeFactory, Polygon, PhysicsShape>,
    RegistryObjectTraits<ShapeFactory, Segment, PhysicsShape>,
    RegistryObjectTraits<ShapeFactory, Capsule, PhysicsShape>,
    RegistryObjectTraits<ShapeFactory, EmptyShape, PhysicsShape>,

    
    RegistryObjectTraits<BodyFactory, CircleBody, PhysicsBody>,
    RegistryObjectTraits<BodyFactory, CapsuleBody, PhysicsBody>,
    RegistryObjectTraits<BodyFactory, LimbBody, PhysicsBody>,
    RegistryObjectTraits<BodyFactory, SegmentBody, PhysicsBody>,
    RegistryObjectTraits<BodyFactory, PolygonBody, PhysicsBody>,

    RegistryObjectTraits<CreatureFactory, DemoCreature, PhysicsCreature>,

    RegistryObjectTraits<TerrainFactory, CircleTerrain, PhysicsTerrain>,
    RegistryObjectTraits<TerrainFactory, PolygonTerrain, PhysicsTerrain>,
    RegistryObjectTraits<TerrainFactory, SegmentTerrain, PhysicsTerrain>,
    RegistryObjectTraits<TerrainFactory, CapsuleTerrain, PhysicsTerrain>
    >;

TYPED_TEST_SUITE(RegistryObjectTest, TestTypes);

TYPED_TEST(RegistryObjectTest, InitDeinit) {
  using Factory = typename TestFixture::Factory;
  using Object = typename TestFixture::Object;
  using Component = typename TestFixture::Component;

  // 1 entity for world
  ASSERT_EQ(registrySize(this->registry), 1);

  auto cfg = Object::Config::defaultConfig();
  auto obj = this->template getFactory<Factory>()->template create<Object>(cfg);

  ASSERT_TRUE(obj);

  EXPECT_EQ(this->registry.template view<Component>().size(), 1);
  obj->remove();
  obj.reset();
  EXPECT_EQ(this->registry.template view<Component>().size(), 0);
  
  // 1 entity for world
  ASSERT_EQ(registrySize(this->registry), 1);

}
