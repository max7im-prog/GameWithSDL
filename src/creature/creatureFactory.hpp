#include "bodyFactory.hpp"
#include "creature/creature/demoCreature.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
class CreatureFactory {
public:
  CreatureFactory(entt::registry &registry, std::shared_ptr<World> world,
                  std::shared_ptr<PhysicsFactory> physicsFactory,
                  std::shared_ptr<BodyFactory> bodyFactory);

  std::shared_ptr<DemoCreature> createDemoCreature(const DemoCreatureConfig& config);

private:
  CreatureFactory() = delete;
  CreatureFactory(CreatureFactory &other) = delete;
  CreatureFactory(CreatureFactory &&other) = delete;
  CreatureFactory &operator=(CreatureFactory &other) = delete;
  CreatureFactory &operator=(CreatureFactory &&other) = delete;

  entt::registry &registry;
  const std::shared_ptr<World> world;
  const std::shared_ptr<PhysicsFactory> physicsFactory;
  const std::shared_ptr<BodyFactory> bodyFactory;
};