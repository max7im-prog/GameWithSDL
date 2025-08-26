#pragma once
#include <entt/entt.hpp>

class RegistryObjectFactory {
public:
  virtual ~RegistryObjectFactory() = 0;

  template <typename T> std::shared_ptr<T> create(const T::Config &config) {
    std::shared_ptr<T> ret = nullptr;
    try {
      ret = tryCreate<T>(config);
    } catch (std::exception &e) {
      return nullptr;
    }
    registerObject<T>(ret);
    return ret;
  }

protected:
  RegistryObjectFactory(entt::registry &registry);
  entt::registry &registry;

  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent);

  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config);

private:
  template <typename T> void registerObject(const std::shared_ptr<T> object) {
    auto ent = registry.create();
    attach<T>(object, ent);
    object->setEntity(ent);
  }
  RegistryObjectFactory() = delete;
  RegistryObjectFactory(RegistryObjectFactory &other) = delete;
  RegistryObjectFactory(RegistryObjectFactory &&other) = delete;
  RegistryObjectFactory operator=(RegistryObjectFactory &other) = delete;
  RegistryObjectFactory operator=(RegistryObjectFactory &&other) = delete;
};