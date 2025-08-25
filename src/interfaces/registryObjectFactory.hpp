#pragma once
#include <entt/entt.hpp>

template <typename T> struct FacturyConfigMapping;

class RegistryObjectFactory {
public:
  virtual ~RegistryObjectFactory() = 0;

  template <typename T>
  std::shared_ptr<T>
  create(const typename FacturyConfigMapping<T>::Config &config);

protected:
  RegistryObjectFactory(entt::registry &registry);
  entt::registry &registry;

  template<typename T>
  void registerObject(const std::shared_ptr<T> object);

private:
  RegistryObjectFactory() = delete;
  RegistryObjectFactory(RegistryObjectFactory &other) = delete;
  RegistryObjectFactory(RegistryObjectFactory &&other) = delete;
  RegistryObjectFactory operator=(RegistryObjectFactory &other) = delete;
  RegistryObjectFactory operator=(RegistryObjectFactory &&other) = delete;
};