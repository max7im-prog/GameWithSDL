#include "bodyFactory.hpp"
#include "demoCreature.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
class CreatureFactory : public RegistryObjectFactory {
public:
  CreatureFactory(entt::registry &registry, std::shared_ptr<World> world,
                  std::shared_ptr<PhysicsFactory> physicsFactory,
                  std::shared_ptr<BodyFactory> bodyFactory);

  std::shared_ptr<DemoCreature>
  createDemoCreature(const DemoCreatureConfig &config);

private:
  void registerCreature(std::shared_ptr<Creature> creature);

  const std::shared_ptr<World> world;
  const std::shared_ptr<PhysicsFactory> physicsFactory;
  const std::shared_ptr<BodyFactory> bodyFactory;
};