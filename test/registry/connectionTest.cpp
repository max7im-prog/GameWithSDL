
#include "circle.hpp"
#include "connectionFactory.hpp"
#include "jointFactory.hpp"
#include "registryTestUtils.hpp"

// 3 entities for world and 2 shapes

// TODO: remove lock() from testing
constexpr size_t INIT_SHAPE_COUNT = 3;

template <typename TObject> class ConnectionEntityTest : public WorldRegistryTest {
public:
  using Component = PhysicsJoint;
  using Factory = JointFactory;
  using Object = TObject;

protected:
  static void SetUpTestSuite() {}
  static void TearDownTestSuite() {}
  void SetUp() override {
    initFactories();
    Circle::Config cfg;
    cfg.defaultConfig();
    shape1 = shapeFactory->create<Circle>(cfg).lock();
    shape2 = shapeFactory->create<Circle>(cfg).lock();
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
using SimpleConnectionEntityTypes =
    ::testing::Types<DistanceConnection, RevoluteConnection, WeldConnection>;

TYPED_TEST_SUITE(ConnectionEntityTest, SimpleConnectionEntityTypes);

TYPED_TEST(ConnectionEntityTest, InitDeinit) {

  using Factory = typename TestFixture::Factory;
  using Object = typename TestFixture::Object;
  using Component = typename TestFixture::Component;
  using Conf = typename Object::Config;

  ASSERT_EQ(WorldRegistryTest::registrySize(this->registry), INIT_SHAPE_COUNT);
  Conf cfg;
  cfg.defaultConfig();
  cfg.templateJointCfg.jointDef.bodyIdA = this->shape1->getBodyId();
  cfg.templateJointCfg.jointDef.bodyIdB = this->shape2->getBodyId();
  auto obj = this->connectionFactory->template create<Object>(cfg).lock();
  ASSERT_TRUE(obj);
  EXPECT_EQ(this->registry.template view<Component>().size(), 1);
  obj->remove();
  obj.reset();
  EXPECT_EQ(this->registry.template view<Component>().size(), 0);
  ASSERT_EQ(WorldRegistryTest::registrySize(this->registry), INIT_SHAPE_COUNT);
}
