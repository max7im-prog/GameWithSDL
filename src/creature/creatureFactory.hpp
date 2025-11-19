#pragma once

#include "bodyFactory.hpp"
#include "connectionFactory.hpp"
#include "creatureComponents.hpp"
#include "demoCreature.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
#include "texturer.hpp"
 

class CreatureFactory : public RegistryObjectFactory<CreatureFactory> {
public:
  CreatureFactory(entt::registry &registry, std::shared_ptr<World> world,
                  std::shared_ptr<BodyFactory> bodyFactory,
                  std::shared_ptr<ConnectionFactory> connectionFactory,
                  std::shared_ptr<Texturer> texturer
                );

  template <typename T> static constexpr bool supports() {
    return std::is_same_v<T, DemoCreature>;
  }

protected:
  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent) {
    auto &comp = registry.emplace_or_replace<PhysicsCreature>(ent);
    comp.creature = object;
    world->addObject(std::static_pointer_cast<RegistryObject>(object));
  }

  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config) {
    auto ret = std::shared_ptr<T>(
        new T(registry, world, config, bodyFactory, connectionFactory));
    return ret;
  }
  template <typename T>
  void setUp(std::shared_ptr<T> object, const T::Config &config) {

    if(config._renderConfig){
      _texturer->setRenderConfig(config._renderConfig);
      object->accept(*(_texturer.get()));
      _texturer->resetRenderConfig();
    }

  }

private:
  const std::shared_ptr<World> world;
  const std::shared_ptr<BodyFactory> bodyFactory;
  const std::shared_ptr<ConnectionFactory> connectionFactory;
  const std::shared_ptr<Texturer> _texturer;

  template <typename> friend class RegistryObjectFactory;
};