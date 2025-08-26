#pragma once
#include "entt/entt.hpp"
#include "registryObjectFactory.hpp"

/**
 * @brief Interface to store objects that have to be registered in
 * entt::registry. Provides composition capabilities.
 *
 */
class RegistryObject {
public:
  virtual ~RegistryObject() = 0;
  entt::entity getEntity();

  /**
   * @brief Checks if an object is a valid entry in registry
   *
   */
  bool isValid() const;

  /**
   * @brief removes object's entry from registry. Propagates the removal to
   * children if has children.
   *
   */
  void remove();

  /**
   * @brief Registers an object as the child. Registering an object as a child
   * propagates the removal of entities to the children upon removal and
   * destruction.
   *
   */
  void registerChild(std::shared_ptr<RegistryObject> child);

  /**
   * @brief Unregisters an object from the child of an object. Registering an
   * object as a child propagates the removal of entities to the children upon
   * removal and destruction of a parent.
   *
   */
  void unregisterChild(std::shared_ptr<RegistryObject> child);

  virtual void update(float dt);

protected:
  RegistryObject(entt::registry &registry);

  void setEntity(entt::entity e);

  entt::registry &registry;
  entt::entity entity;

private:
  /**
   * @brief This value is a unique pointer to avoid additional memory allocation
   * for objects with no children.
   *
   */
  std::unique_ptr<std::vector<std::shared_ptr<RegistryObject>>> children;

  RegistryObject() = delete;
  RegistryObject(RegistryObject &other) = delete;
  RegistryObject(RegistryObject &&other) = delete;
  RegistryObject &operator=(RegistryObject &other) = delete;
  RegistryObject &operator=(RegistryObject &&other) = delete;

  template <typename Derived>
  friend class RegistryObjectFactory;
};