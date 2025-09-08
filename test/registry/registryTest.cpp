#include "bodyComponents.hpp"
#include "bodyFactory.hpp"
#include "capsuleBody.hpp"
#include "creatureComponents.hpp"
#include "demoCreature.hpp"
#include "game.hpp"
#include "limbBody.hpp"
#include "physicsComponents.hpp"
#include "polygon.hpp"
#include "polygonBody.hpp"
#include "registryUtils.hpp"
#include "segment.hpp"
#include "shapeFactory.hpp"
#include "terrainFactory.hpp"
#include "gtest/gtest.h"
#include <gtest/gtest.h>

// Traits: bind Factory, Object, and Component together
template <typename TFactory, typename TObject, typename TComponent>
struct RegistryObjectTraits {
  using Factory = TFactory;
  using Object = TObject;
  using Component = TComponent;
};

template <typename Traits>
class StandaloneEntityTest : public WorldRegistryTest {
public:
  using Factory = typename Traits::Factory;
  using Object = typename Traits::Object;
  using Component = typename Traits::Component;


protected:
  static void SetUpTestSuite() {} // <- corrected name
  static void TearDownTestSuite() {}
  void SetUp() override { initFactories(); }
  void TearDown() override {
    world.reset();
    registry.clear();
    deinitFactories();
  }
};

// List of (Factory, Object, Component) triples to test
using StandaloneEntityTypes = ::testing::Types<

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
    RegistryObjectTraits<TerrainFactory, CapsuleTerrain, PhysicsTerrain>>;

TYPED_TEST_SUITE(StandaloneEntityTest, StandaloneEntityTypes);

TYPED_TEST(StandaloneEntityTest, InitDeinit) {
  using Factory = typename TestFixture::Factory;
  using Object = typename TestFixture::Object;
  using Component = typename TestFixture::Component;

  // 1 entity for world
  ASSERT_EQ(WorldRegistryTest::registrySize(this->registry), 1);

  auto cfg = Object::Config::defaultConfig();
  auto obj = this->template getFactory<Factory>()->template create<Object>(cfg);

  ASSERT_TRUE(obj);

  EXPECT_EQ(this->registry.template view<Component>().size(), 1);
  obj->remove();
  obj.reset();
  EXPECT_EQ(this->registry.template view<Component>().size(), 0);

  // 1 entity for world
  EXPECT_EQ(WorldRegistryTest::registrySize(this->registry), 1);
}
