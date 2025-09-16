#include "circle.hpp"
#include "distanceJoint.hpp"
#include "jointFactory.hpp"
#include "mouseJoint.hpp"
#include "registryTestUtils.hpp"
#include "revoluteJoint.hpp"

// 3 entities for world and 2 shapes
constexpr size_t INIT_SHAPE_COUNT = 3;

template <typename TObject> class JointEntityTest : public WorldRegistryTest {
public:
  using Component = PhysicsJoint;
  using Factory = JointFactory;
  using Object = TObject;

protected:
  static void SetUpTestSuite() {}
  static void TearDownTestSuite() {}
  void SetUp() override {
    initFactories();
    auto cfg = Circle::Config::defaultConfig();
    shape1 = shapeFactory->create<Circle>(cfg);
    shape2 = shapeFactory->create<Circle>(cfg);
  }
  void TearDown() override {
    shape1.reset();
    shape2.reset();
    world.reset();
    registry.clear();
    deinitFactories();
  }
  std::shared_ptr<Circle> shape1;
  std::shared_ptr<Circle> shape2;
};

// List of (Factory, Object, Component) triples to test
using JointEntityTypes =
    ::testing::Types<RevoluteJoint, DistanceJoint, MouseJoint, WeldJoint>;

TYPED_TEST_SUITE(JointEntityTest, JointEntityTypes);

TYPED_TEST(JointEntityTest, InitDeinit) {

  using Factory = typename TestFixture::Factory;
  using Object = typename TestFixture::Object;
  using Component = typename TestFixture::Component;

  ASSERT_EQ(WorldRegistryTest::registrySize(this->registry), INIT_SHAPE_COUNT);
  auto cfg = Object::Config::defaultConfig();
  cfg.jointDef.bodyIdA = this->shape1->getBodyId();
  cfg.jointDef.bodyIdB = this->shape2->getBodyId();
  auto obj = this->jointFactory->template create<Object>(cfg);
  ASSERT_TRUE(obj);
  EXPECT_EQ(this->registry.template view<Component>().size(), 1);
  obj->remove();
  obj.reset();
  EXPECT_EQ(this->registry.template view<Component>().size(), 0);
  ASSERT_EQ(WorldRegistryTest::registrySize(this->registry), INIT_SHAPE_COUNT);
}
