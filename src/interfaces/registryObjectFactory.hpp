#pragma once
#include <entt/entt.hpp>

/**
 * @brief A concept to enforce creation constraints onto a factory.
 *
 * @tparam T An object being created
 * @tparam Factory A factory that creates an object
 */
template <typename T, typename Factory>
concept FactoryConstructable = requires {
  typename T::Config;
  { Factory::template supports<T>() } -> std::convertible_to<bool>;
} && Factory::template supports<T>();

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
   * @tparam T Class that is being created
   * @param config A configuration defined in T as T::Config
   */
  template <typename T>
    requires FactoryConstructable<T, Derived>
  std::weak_ptr<T> create(const T::Config &config) {
    std::shared_ptr<T> ret = nullptr;
    try {
      ret = derived().template tryCreate<T>(config);
    } catch (std::exception &e) {
      // TODO: log error
      return std::weak_ptr<T>();
    }
    if (!ret) {
      // TODO: log error
      return std::weak_ptr<T>();
    }
    derived().template registerObject<T>(ret);
    derived().template setUp<T>(ret, config);
    return ret;
  }

  /**
   * @brief Determines whether a type T is allowed to be created by the factory.
   *
   * This function must be implemented by each CRTP-derived factory class.
   * It is used by `create<T>()` to enforce compile-time constraints.
   *
   * @tparam T The type of object to check.
   * @return true if T can be created by the factory, false otherwise.
   */
  template <typename T> static constexpr bool supports();

protected:
  RegistryObjectFactory(entt::registry &registry) : registry(registry) {}
  entt::registry &registry;

  /**
   * @brief A member function to attach created object to an entity in a
   * registry.
   *
   * Has to be implemented by derived classes for creation to
   * work. Sample implementation looks like this:
   * @code{.cpp}
   * {auto &comp = registry.emplace_or_replace<PhysicsConnection>(ent);
   * comp.connection = object;}
   * @endcode
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
   * @code {.cpp}
   * {return std::shared_ptr<T>(new T(registry, world, config, shapeFactory,
   * jointFactory));}
   * @endcode
   *
   * @tparam T Class that is being created
   * @param config A configuration defined in T as T::Config
   */
  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config);

  /**
   * @brief A function to set up the additional fields, parameters and
   * components of an object. Each factory may implement it as it sees fit.
   *
   * @tparam T class of an object
   * @param object
   * @param config object's configuration that was used during
   * factory.create(config)
   */
  template <typename T>
  void setUp(std::shared_ptr<T> object, const T::Config &config);

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