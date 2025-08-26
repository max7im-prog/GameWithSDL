#pragma once
#include <entt/entt.hpp>

/**
 * @brief Factory interface for all factories that have to create
 * registryObjects.
 *
 * Interface uses create() to implement a creation process. For it to work,
 * derived classes have to implement tryCreate() and attach() member functions.
 *
 * @tparam Derived The name of a derived class. Used to solve CRTP problem
 * (https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)
 */
template <typename Derived> class RegistryObjectFactory {
public:
  virtual ~RegistryObjectFactory() = default;

  /**
   * @brief Template for object creation.
   *
   * For it to work, derived classes have to implement tryCreate() and attach()
   * member functions.
   *
   * @tparam T Class that is beig created
   * @param config A configuration defined in T as T::Config
   */
  template <typename T> std::shared_ptr<T> create(const T::Config &config) {
    std::shared_ptr<T> ret = nullptr;
    try {
      ret = derived().template tryCreate<T>(config);
    } catch (std::exception &e) {
      return nullptr;
    }
    derived().template registerObject<T>(ret);
    return ret;
  }

protected:
  RegistryObjectFactory(entt::registry &registry) : registry(registry) {}
  entt::registry &registry;

  /**
   * @brief A member function to attach created object to an entity in a
   * registry.
   *
   * Has to be implemented by derived classes for creation to
   * work. Sample implementation looks like this:
   * {auto &comp = registry.emplace_or_replace<PhysicsConnection>(ent);
   * comp.connection = object;}
   *
   *
   * @tparam T Class that is being created
   * @param object Instance of a class T that has been created in create()
   * @param ent Entity created in registerObject()
   */
  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent);

  /**
   * @brief A member function to create an instance of a class that is being
   * created.
   *
   * Has to be implemented by derived classes for creation to
   * work. Sample implementation looks like this:
   * {return std::shared_ptr<T>(new T(registry, world, config, shapeFactory,
   * jointFactory));}
   *
   * @tparam T Class that is being created
   * @param config A configuration defined in T as T::Config
   */
  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config);

private:
  /**
   * @brief a function to register a registryObject in entt::registry
   */
  template <typename T> void registerObject(const std::shared_ptr<T> object) {
    auto ent = registry.create();
    derived().template attach<T>(object, ent);
    object->setEntity(ent);
  }
  RegistryObjectFactory() = delete;
  RegistryObjectFactory(RegistryObjectFactory &other) = delete;
  RegistryObjectFactory(RegistryObjectFactory &&other) = delete;
  RegistryObjectFactory operator=(RegistryObjectFactory &other) = delete;
  RegistryObjectFactory operator=(RegistryObjectFactory &&other) = delete;

private:
  Derived &derived() { return static_cast<Derived &>(*this); }
};