#include "entt/entt.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(BasicTest, TrueEqTrue) { EXPECT_EQ(true, true); }

TEST(BasicTest, OneMore) { EXPECT_EQ(false, false); }

TEST(BasicTest, RegistryNull) {
  entt::registry registry;
  auto a = registry.valid(entt::null);
}
