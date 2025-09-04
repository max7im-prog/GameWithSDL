#pragma once
#include "basicWorld.hpp"
#include "physicsComponents.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
class WorldFactory : public RegistryObjectFactory<WorldFactory> {
public:
  WorldFactory(entt::registry &registry);

  template <typename T> static constexpr bool supports() {
    return std::is_same_v<T, BasicWorld>;
  }

protected:
  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent) {
    auto &comp = registry.emplace_or_replace<PhysicsWorld>(ent);
    comp.world = object;
  }

  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config) {
    auto ret = std::shared_ptr<T>(new T(registry, config));
    return ret;
  }

private:
  template <typename> friend class RegistryObjectFactory;
};