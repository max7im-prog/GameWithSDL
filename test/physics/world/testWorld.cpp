
/* #include "basicWorld.hpp"
#include "entt/entity/fwd.hpp"
#include "physicsComponents.hpp"
#include "world.hpp" // include your header
#include "worldFactory.hpp"
#include <gtest/gtest.h>

// Fixture example if world has some state
class WorldTest : public ::testing::Test {
protected:
  static void setupTestSuite() {}

  void SetUp() override {}

  void TearDown() override {}
  static void TearDownTestSuite() {}

  std::shared_ptr<World> world = nullptr;
};

TEST_F(WorldTest, InitTest) {
  entt::registry registry; // non-static is safer
  std::shared_ptr<WorldFactory> worldFactory;
  worldFactory = std::shared_ptr<WorldFactory>(new WorldFactory(registry));
  {
    auto config = BasicWorld::Config::defaultConfig();
    world = worldFactory->create<BasicWorld>(config);
  }

  // Check that PhysicsWorld component exists
  size_t size = registry.view<PhysicsWorld>().size();
  ASSERT_EQ(size, 1);

  // Clear registry
  registry.clear();

  // Check that no PhysicsWorld components remain
  size = registry.view<PhysicsWorld>().size();
  ASSERT_EQ(size, 0);
} */