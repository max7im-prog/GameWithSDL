#include "bodyFactory.hpp"
#include "connectionFactory.hpp"
#include "creatureComponents.hpp"
#include "demoCreature.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
class CreatureFactory : public RegistryObjectFactory<CreatureFactory> {
public:
  CreatureFactory(entt::registry &registry, std::shared_ptr<World> world,
                  std::shared_ptr<BodyFactory> bodyFactory,
                  std::shared_ptr<ConnectionFactory> connectionFactory);

  std::shared_ptr<DemoCreature>
  createDemoCreature(const DemoCreatureConfig &config);

protected:
  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent) {
    auto &comp = registry.emplace_or_replace<PhysicsCreature>(ent);
    comp.creature = object;
  }

  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config) {
    return std::shared_ptr<T>(
        new T(registry, world, config, bodyFactory, connectionFactory));
  }

private:
  const std::shared_ptr<World> world;
  const std::shared_ptr<BodyFactory> bodyFactory;
  const std::shared_ptr<ConnectionFactory> connectionFactory;

  template <typename>
friend class RegistryObjectFactory;
};