#pragma once
#include "registryObject.hpp"
#include "registryObjectFactory.hpp"

/**
 * @brief Interface to store composite objects that have to be registered in
 * entt::registry.
 *
 */
class RegistryComposite : public RegistryObject {
public:
  virtual ~RegistryComposite();

  /**
   * @brief Removes object's entry from registry. Propagates the removal to
   * children if has children.
   *
   */
  virtual void remove();

  /**
   * @brief Removes all children of an object from the registry. Removal is
   * propagated to children's children.
   *
   */
  void removeChildren();

  /**
   * @brief Registers an object as the child.
   *
   * Registering an object as a child propagates the removal of entities to the
   * children upon removal and destruction.
   *
   */
  virtual void registerChild(std::weak_ptr<RegistryComposite> child);

  /**
   * @brief Unregisters an object from the child of an object.
   *
   * Registering an object as a child propagates the removal of entities to the
   * children upon removal and destruction of a parent.
   *
   */
  virtual void unregisterChild(std::weak_ptr<RegistryComposite> child);

  virtual void update(float dt);

protected:
  RegistryComposite(entt::registry &registry);
  std::vector<std::weak_ptr<RegistryComposite>> getChildren();

private:
  /**
   * @brief This value is a unique pointer to avoid additional memory allocation
   * for objects with no children.
   *
   */
  std::unique_ptr<std::vector<std::weak_ptr<RegistryComposite>>> children;
};
